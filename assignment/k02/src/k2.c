#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define MATRIX_FILENAME "k2-input1.csv"
#define VECTOR_FILENAME "k2-input2.csv"

double *matrix_vector_multiply(const double **matrix, const double *vector,
                               int rows, int cols) {
    double *result = allocVector(cols);
    int i, j;

    for (i = 0; i < cols; i++) result[i] = 0.0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    return result;
}

int main(void) {
    FILE *matrix_fin, *vector_fin;
    double **matrix, **tmp, *vector, *result;
    int rows, cols, vector_size, vector_cols, i;

    if ((matrix_fin = fopen(MATRIX_FILENAME, "r")) == NULL) {
        fprintf(stderr, "file %s is not found.\n", MATRIX_FILENAME);
        return EXIT_FAILURE;
    }
    if ((vector_fin = fopen(VECTOR_FILENAME, "r")) == NULL) {
        fprintf(stderr, "file %s is not found.\n", VECTOR_FILENAME);
        return EXIT_FAILURE;
    }

    // Error handlings are processed in csvRead(), so omitted here.
    matrix = csvRead(&rows, &cols, matrix_fin);
    tmp = csvRead(&vector_size, &vector_cols, vector_fin);
    fclose(matrix_fin);
    fclose(vector_fin);
    vector = matrix2colVector((const double **)tmp, vector_size, 1);
    freeMatrix(tmp);

    if (cols != vector_size || vector_cols != 1) {
        printf("Error: Matrix and vector sizes do not match.\n");
        freeVector(vector);
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }

    // cols == vector_size
    result =
        matrix_vector_multiply((const double **)matrix, vector, rows, cols);

    for (i = 0; i < vector_size; i++) {
        printf("%.4f\n", result[i]);
    }
    freeVector(result);
    freeVector(vector);
    freeMatrix(matrix);

    return EXIT_SUCCESS;
}