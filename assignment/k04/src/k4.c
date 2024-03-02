#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define MATRIX_FILENAME "k4-input1.csv"
#define VECTOR_FILENAME "k4-input2.csv"

double **copy_matrix(const double **src, double **dst, int row, int col) {
    int i;

    for (i = 0; i < row; i++) {
        memcpy(dst[i], src[i], sizeof(double) * col);
    }
    return dst;
}

int swap_matrix_row(double **matrix, int row, int i, int j) {
    double *tmp;

    if (row <= i || row <= j) {
        fprintf(stderr,
                "Error: row index is out of range. (row=%d, i=%d, j=%d)\n", row,
                i, j);
        return -1;
    }
    if (i == j) return 0;  // nothing to do

    tmp = matrix[i];
    matrix[i] = matrix[j];
    matrix[j] = tmp;

    return 0;
}

int swap_vector_element(double *vector, int vector_size, int i, int j) {
    double tmp;

    if (vector_size <= i || vector_size <= j) {
        fprintf(stderr,
                "Error: index is out of range. (vector_size=%d, i=%d, j=%d)\n",
                vector_size, i, j);
        return -1;
    }
    if (i == j) return 0;  // nothing to do

    tmp = vector[i];
    vector[i] = vector[j];
    vector[j] = tmp;

    return 0;
}

double *solve_simultaneous_equations(const double **org_matrix,
                                     const double *org_vector, int row,
                                     int col) {
    double *result = allocVector(col), *vector = allocVector(col);
    double **matrix = allocMatrix(row, col);
    int i, j, k;

    // vector and matrix copied to cause no side effects.
    memcpy(vector, org_vector, sizeof(double) * col);
    copy_matrix(org_matrix, matrix, row, col);

    for (i = 0; i < row; i++) {
        size_t max_row = 0;
        double max = 0.0;
        // find maximum absolute value in column i from row i to row end.
        for (j = i; j < row; j++) {
            if (max < fabs(matrix[j][i])) {
                max = fabs(matrix[j][i]);
                max_row = j;
            }
        }

        if (swap_matrix_row(matrix, row, i, max_row)) {
            fprintf(stderr, "Error: swap_matrix_row() failed.\n");
            freeVector(vector);
            freeMatrix(matrix);
            return NULL;
        }
        if (swap_vector_element(vector, col, i, max_row)) {
            fprintf(stderr, "Error: swap_vector_element() failed.\n");
            freeVector(vector);
            freeMatrix(matrix);
            return NULL;
        }

        // forward elimination
        for (j = i + 1; j < row; j++) {
            double tmp = matrix[j][i] / matrix[i][i];

            for (k = i; k < col; k++) {
                matrix[j][k] -= matrix[i][k] * tmp;
            }
            vector[j] -= vector[i] * tmp;
        }
    }

    // backward substitution
    for (i = row - 1; i >= 0; i--) {
        double row_value = vector[i];

        for (j = col - 1; j > i; j--) {
            row_value -= matrix[i][j] * result[j];
        }
        result[i] = row_value / matrix[i][i];
    }

    freeVector(vector);
    freeMatrix(matrix);
    return result;
}

int main(void) {
    FILE *matrix_fin, *vector_fin;
    double **matrix, **tmp, *vector, *result;
    int row, col, vector_size, vector_col, i;

    if ((matrix_fin = fopen(MATRIX_FILENAME, "r")) == NULL) {
        fprintf(stderr, "file %s is not found.\n", MATRIX_FILENAME);
        return EXIT_FAILURE;
    }
    if ((vector_fin = fopen(VECTOR_FILENAME, "r")) == NULL) {
        fprintf(stderr, "file %s is not found.\n", VECTOR_FILENAME);
        return EXIT_FAILURE;
    }

    // Error handlings are processed in csvRead(), so omitted here.
    matrix = csvRead(&row, &col, matrix_fin);
    tmp = csvRead(&vector_size, &vector_col, vector_fin);
    fclose(matrix_fin);
    fclose(vector_fin);
    vector = matrix2colVector((const double **)tmp, vector_size, 1);
    freeMatrix(tmp);

    if (col != vector_size || vector_col != 1) {
        printf("Error: Matrix and vector sizes do not match.\n");
        freeVector(vector);
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }

    // col == vector_size
    result =
        solve_simultaneous_equations((const double **)matrix, vector, row, col);

    if (result == NULL) {
        fprintf(stderr, "Error: solve_simultaneous_equations() failed.\n");
        freeVector(vector);
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }

    for (i = 0; i < vector_size; i++) {
        printf("%.4f\n", result[i]);
    }

    freeVector(result);
    freeVector(vector);
    freeMatrix(matrix);

    return EXIT_SUCCESS;
}