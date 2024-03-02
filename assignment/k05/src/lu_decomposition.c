#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"
#include "k5.h"

static void init_lu_decomposition(const double **org_matrix, size_t matrix_size,
                                  lu_decomposition_t *lu) {
    size_t i, j;

    lu->l_matrix = allocMatrix(matrix_size, matrix_size);
    lu->p_matrix = allocMatrix(matrix_size, matrix_size);
    lu->u_matrix = allocMatrix(matrix_size, matrix_size);

    copy_matrix(org_matrix, lu->u_matrix, matrix_size, matrix_size);

    for (i = 0; i < matrix_size; i++) {
        for (j = 0; j < matrix_size; j++) {
            lu->l_matrix[i][j] = 0.0;
            lu->p_matrix[i][j] = 0.0;
        }
    }
    for (i = 0; i < matrix_size; i++) {
        lu->l_matrix[i][i] = lu->p_matrix[i][i] = 1.0;
    }
}

static void free_lu_decomposition(lu_decomposition_t *lu) {
    freeMatrix(lu->l_matrix);
    freeMatrix(lu->u_matrix);
    freeMatrix(lu->p_matrix);
}

static int lu_decomposition(const double **org_matrix, size_t matrix_size,
                            lu_decomposition_t *lu) {
    size_t i, j, k;

    init_lu_decomposition(org_matrix, matrix_size, lu);

    for (i = 0; i < matrix_size; i++) {
        size_t max_row = 0;
        double max = 0.0;
        // find maximum absolute value in column i from row i to row end.
        for (j = i; j < matrix_size; j++) {
            if (max < fabs(lu->u_matrix[j][i])) {
                max = fabs(lu->u_matrix[j][i]);
                max_row = j;
            }
        }

        if (swap_matrix_row(lu->u_matrix, matrix_size, i, max_row)) {
            fprintf(stderr, "Error: swap_matrix_row(lu->u_matrix) failed.\n");
            return -1;
        }
        if (swap_matrix_row(lu->p_matrix, matrix_size, i, max_row)) {
            fprintf(stderr, "Error: swap_matrix_row(p_matrix) failed.\n");
            return -1;
        }
        // swap l_matrix value only calculated (ignore diagonal elements)
        for (j = 0; j < i; j++) {
            double tmp = lu->l_matrix[i][j];
            lu->l_matrix[i][j] = lu->l_matrix[max_row][j];
            lu->l_matrix[max_row][j] = tmp;
        }

        // forward elimination
        for (j = i + 1; j < matrix_size; j++) {
            double tmp = lu->u_matrix[j][i] / lu->u_matrix[i][i];

            for (k = i; k < matrix_size; k++) {
                lu->u_matrix[j][k] -= lu->u_matrix[i][k] * tmp;
            }
            lu->l_matrix[j][i] = tmp;
        }
    }
    return 0;
}

double *solve_simultaneous_by_lu_decomposition(const double **org_matrix,
                                               const double *org_vector,
                                               size_t matrix_size,
                                               lu_decomposition_t *lu) {
    double *vector = allocVector(matrix_size),
           *result = allocVector(matrix_size);
    size_t i, j;

    // vector and matrix copied to cause no side effects.

    // LU decomposition
    if (lu_decomposition(org_matrix, matrix_size, lu)) {
        fprintf(stderr, "Error: lu_decomposition() failed.\n");
        freeVector(vector);
        freeVector(result);
        free_lu_decomposition(lu);
        return NULL;
    }

    // solve simultaneous equations
    for (i = 0; i < matrix_size; i++) {
        size_t p_index = 0;
        // get the index with a value 1 in p_matrix
        for (j = 0; j < matrix_size; j++) {
            if (lu->p_matrix[i][j] == 1.0) {
                p_index = j;
                break;
            }
        }
        vector[i] = org_vector[p_index];

        for (j = 0; j < i; j++) {
            vector[i] -= lu->l_matrix[i][j] * vector[j];
        }
    }

    for (i = 0; i < matrix_size; i++) {
        size_t i_inv = matrix_size - 1 - i;
        result[i_inv] = vector[i_inv];
        for (j = i_inv + 1; j < matrix_size; j++) {
            result[i_inv] -= lu->u_matrix[i_inv][j] * result[j];
        }
        result[i_inv] /= lu->u_matrix[i_inv][i_inv];
    }

    freeVector(vector);
    return result;
}