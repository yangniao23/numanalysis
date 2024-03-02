#include <stdio.h>

#include "NAbasic.h"

#define MATRIX_FILENAME "k4-input1.csv"

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

int main(void) {
    FILE *matrix_fin;
    double **matrix;
    int row, col;

    if ((matrix_fin = fopen(MATRIX_FILENAME, "r")) == NULL) {
        fprintf(stderr, "file %s is not found.\n", MATRIX_FILENAME);
        return EXIT_FAILURE;
    }

    // Error handlings are processed in csvRead(), so omitted here.
    matrix = csvRead(&row, &col, matrix_fin);
    fclose(matrix_fin);

    showMatrix((const double **)matrix, row, col);
    putchar('\n');
    swap_matrix_row(matrix, row, 0, 2);
    showMatrix((const double **)matrix, row, col);
    freeMatrix(matrix);

    return EXIT_SUCCESS;
}