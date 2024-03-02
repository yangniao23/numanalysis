#include "k5.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define MATRIX_FILENAME "k5-input1.csv"
#define VECTOR_FILENAME "k5-input2.csv"

int main(void) {
    FILE *matrix_fin, *vector_fin;
    double **matrix, **tmp, *vector, *result;
    lu_decomposition_t lu;
    size_t row, col, vector_size, vector_col, i;

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

    // row == col == vector_size
    result = solve_simultaneous_by_lu_decomposition(
        (const double **)matrix, (const double *)vector, row, &lu);
    if (result == NULL) {
        fprintf(stderr, "Error: solve_simultaneous_equations() failed.\n");
        freeVector(vector);
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }

    // show l matrix
    puts("L =");
    showMatrix((const double **)lu.l_matrix, row, col);
    putchar('\n');

    // show u matrix
    puts("U =");
    showMatrix((const double **)lu.u_matrix, row, col);
    putchar('\n');

    // show p matrix
    puts("P =");
    showMatrix((const double **)lu.p_matrix, row, col);
    putchar('\n');

    // show result vector
    puts("X =");
    for (i = 0; i < row; i++) {
        printf("%f\n", result[i]);
    }

    freeVector(result);
    freeVector(vector);
    freeMatrix(matrix);

    return EXIT_SUCCESS;
}