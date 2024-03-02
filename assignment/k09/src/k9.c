#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define DATA_FILENAME "k9-input_test.csv"
#define X 2.0

static SquareMatrix *calculate_difference_table(Matrix *matrix, size_t n) {
    static SquareMatrix *table = NULL;
    static size_t prev_n = 0;
    size_t i = 0, j = 0;

    if (table == NULL) {
        if ((table = allocSquareMatrix(n)) == NULL) {
            fprintf(stderr, "Failed to allocate memory.\n");
            return NULL;
        }
    } else {
        if ((table = resizeSquareMatrix(table, n)) == NULL) {
            fprintf(stderr, "Failed to allocate memory.\n");
            return NULL;
        }
    }

    for (i = prev_n; i < n; i++) table->data[i][0] = matrix->data[i][1];

    for (i = 1; i < n; i++) {
        size_t j0 = (prev_n > i) ? prev_n - i : 0;

        for (j = j0; j < n - i; j++) {
            table->data[j][i] =
                (table->data[j + 1][i - 1] - table->data[j][i - 1]) /
                (matrix->data[j + i][0] - matrix->data[j][0]);
        }
    }

    prev_n = n;  // store n for next call

    return table;
}

double newton_interpolation(Matrix *matrix, double x, size_t n) {
    SquareMatrix *table;
    double result = 0;
    size_t i, j;

    if ((table = calculate_difference_table(matrix, n)) == NULL) {
        fprintf(stderr, "Error: calculate_difference_table() failed.\n");
        return 0;
    }

    for (i = 0; i < n; i++) {
        double product = 1;
        for (j = 0; j < i; j++) {
            product *= x - matrix->data[j][0];
        }
        result += product * table->data[0][i];
    }

    return result;
}

Matrix *readCsvToMatrix(const char *matrix_filename) {
    FILE *matrix_fin;
    Matrix *matrix;

    if ((matrix_fin = fopen(matrix_filename, "r")) == NULL) {
        fprintf(stderr, "file %s is not found.\n", matrix_filename);
        return NULL;
    }

    // Error handlings are not processed in csvRead(), so check not null here.
    matrix = csvRead(matrix_fin);
    fclose(matrix_fin);
    if (matrix == NULL) {
        fprintf(stderr, "Error: csvRead() failed.\n");
        return NULL;
    }

    return matrix;
}

int main(void) {
    Matrix *matrix;
    double res;

    if ((matrix = readCsvToMatrix(DATA_FILENAME)) == NULL) {
        return EXIT_FAILURE;
    }

    if (matrix->col != 2) {
        fprintf(stderr, "Error: col size of matrix must be 2.\n");
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }

    res = newton_interpolation(matrix, X, 3);
    printf("1: %f\n", res);
    printf("2: %f\n", newton_interpolation(matrix, X, 4));

    freeMatrix(matrix);
    return EXIT_SUCCESS;
}