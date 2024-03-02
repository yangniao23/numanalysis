#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define MATRIX_FILENAME "k6-input1.csv"
#define VECTOR_FILENAME "k6-input2.csv"
#define JACOBI_LOOP_MAX 84
#define GAUSS_SEIDAL_LOOP_MAX 15

void init_vectors(double **result, size_t size) {
    size_t i;

    result[0] = allocVector(size);
    result[1] = allocVector(size);

    for (i = 0; i < size; i++) {
        result[0][i] = 0.0;
        result[1][i] = 0.0;
    }
}

double *jacobi_method(const double **matrix, const double *vector, size_t size,
                      size_t loop_max) {
    double *result[2];
    size_t i, j, k;

    init_vectors(result, size);

    for (i = 0; i < loop_max; i++) {
        for (j = 0; j < size; j++) {
            double tmp = 0.0;
            for (k = 0; k < size; k++) {
                if (j == k) continue;  // skip diagonal elements
                tmp += matrix[j][k] * result[i % 2][k];
            }
            result[(i + 1) % 2][j] = (vector[j] - tmp) / matrix[j][j];
        }
    }

    // last result vector is stored in result[loop_max % 2] (i_max = loop_max -
    // 1)
    freeVector(result[(loop_max + 1) % 2]);
    return result[loop_max % 2];
}

double *gauss_seidal_method(const double **matrix, const double *vector,
                            size_t size, size_t loop_max) {
    double *result[2];
    size_t i, j, k;

    init_vectors(result, size);

    for (i = 0; i < loop_max; i++) {
        for (j = 0; j < size; j++) {
            double tmp = 0.0;
            // lower triangular matrix elements are already updated
            for (k = 0; k < j; k++) {
                tmp += matrix[j][k] * result[(i + 1) % 2][k];
            }
            // skip diagonal elements
            // upper triangular matrix elements are not updated yet
            for (k = j + 1; k < size; k++) {
                tmp += matrix[j][k] * result[i % 2][k];
            }
            result[(i + 1) % 2][j] = (vector[j] - tmp) / matrix[j][j];
        }
    }

    // last result vector is stored in result[loop_max % 2] (i_max = loop_max -
    // 1)
    freeVector(result[(loop_max + 1) % 2]);
    return result[loop_max % 2];
}

int read_matrix_and_vector(const char *matrix_filename,
                           const char *vector_filename, double ***matrix,
                           double **vector, size_t *size) {
    FILE *matrix_fin, *vector_fin;
    double **tmp;
    size_t row, col, vector_size, vector_col;

    if ((matrix_fin = fopen(matrix_filename, "r")) == NULL) {
        fprintf(stderr, "file %s is not found.\n", matrix_filename);
        return -1;
    }
    if ((vector_fin = fopen(vector_filename, "r")) == NULL) {
        fprintf(stderr, "file %s is not found.\n", vector_filename);
        return -1;
    }

    // Error handlings are processed in csvRead(), so omitted here.
    *matrix = csvRead(&row, &col, matrix_fin);
    tmp = csvRead(&vector_size, &vector_col, vector_fin);
    fclose(matrix_fin);
    fclose(vector_fin);
    *vector = matrix2colVector((const double **)tmp, vector_size, 1);
    freeMatrix(tmp);

    if ((row != col) || (col != vector_size) || (vector_col != 1)) {
        printf("Error: Matrix and vector sizes do not match.\n");
        freeVector(*vector);
        freeMatrix(*matrix);
        return -1;
    }

    *size = row;  // row == col == vector_size
    return 0;
}

int main(void) {
    double **matrix, *vector, *jacobi_result, *gauss_seidal_result;
    size_t size, i;

    if (read_matrix_and_vector(MATRIX_FILENAME, VECTOR_FILENAME, &matrix,
                               &vector, &size) != 0) {
        return EXIT_FAILURE;
    }

    jacobi_result =
        jacobi_method((const double **)matrix, vector, size, JACOBI_LOOP_MAX);
    gauss_seidal_result = gauss_seidal_method((const double **)matrix, vector,
                                              size, GAUSS_SEIDAL_LOOP_MAX);

    if ((jacobi_result == NULL) || (gauss_seidal_result == NULL)) {
        fprintf(stderr, "Error: solving failed.\n");
        freeVector(vector);
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }

    printf("Jacobi method: loop num = %d\n", JACOBI_LOOP_MAX);
    for (i = 0; i < size; i++) {
        printf("%f\n", jacobi_result[i]);
    }
    printf("\n");

    printf("Gauss-Seidal method: loop num = %d\n", GAUSS_SEIDAL_LOOP_MAX);
    for (i = 0; i < size; i++) {
        printf("%f\n", gauss_seidal_result[i]);
    }
    printf("\n");

    freeVector(jacobi_result);
    freeVector(gauss_seidal_result);
    freeVector(vector);
    freeMatrix(matrix);

    return EXIT_SUCCESS;
}