#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define PARAM_FILENAME "k12-input.csv"

typedef double (*F)(double, double);

// df = dy/dx = -xy + x
double df(double x, double y) { return -x * y + x; }

Matrix *euler_method(F f, double x0, double width, double x_inf, double y0) {
    size_t len = (size_t)((x_inf - x0) / width) + 1;  // len = n + 1
    Matrix *matrix = allocMatrix(2, len);  // data[0][i] = x_i, data[1][i] = y_i
    size_t i;

    if (matrix == NULL) {
        fprintf(stderr, "Error: allocMatrix() failed.\n");
        return NULL;
    }

    matrix->data[0][0] = x0;
    matrix->data[1][0] = y0;
    for (i = 1; i < len; i++) {
        matrix->data[0][i] = matrix->data[0][i - 1] + width;
        matrix->data[1][i] =
            matrix->data[1][i - 1] +
            width * f(matrix->data[0][i - 1], matrix->data[1][i - 1]);
    }

    return matrix;
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

bool validate(const Matrix *matrix) {
    // matrix[0] = {x0, width, x_inf, y_0}
    if (matrix->row != 1) {
        fprintf(stderr, "Error: row size of matrix must be 1.\n");
        return false;
    }
    if (matrix->col != 4) {
        fprintf(stderr, "Error: col size of matrix must be 3.\n");
        return false;
    }
    if (matrix->data[0][1] < 0) {
        fprintf(stderr, "Error: step width must be positive value.\n");
        return false;
    }
    return true;
}

int main(void) {
    Matrix *matrix, *res;

    if ((matrix = readCsvToMatrix(PARAM_FILENAME)) == NULL) {
        return EXIT_FAILURE;
    }
    if (!validate(matrix)) {
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }

    printf("step width: %f\n", matrix->data[0][1]);

    res = euler_method(df, matrix->data[0][0], matrix->data[0][1],
                       matrix->data[0][2], matrix->data[0][3]);

    if (res == NULL) {
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < res->col; i++) {
        printf("x=%.7f,y=%.7f\n", res->data[0][i], res->data[1][i]);
    }

    freeMatrix(res);
    freeMatrix(matrix);

    return EXIT_SUCCESS;
}