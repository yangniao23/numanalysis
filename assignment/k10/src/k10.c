#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define VECTOR_FILENAME "k10-input.csv"

typedef double (*F)(double);

double sq(double x) { return x * x; }

double trapezoid(F f, double a, double b, size_t n) {
    double h = (b - a) / n;
    double sum = 0.0;
    size_t i;

    // sum = h*(f(a)/2 + f(a + h) + f(a + 2h) + ... + f(a + (n - 1)h) + f(b)/2)
    // where (a + nh = b)
    for (i = 1; i < n; i++) sum += f(a + i * h);
    sum += (f(a) + f(b)) / 2.0;

    return h * sum;
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
    if (matrix->row != 1) {
        fprintf(stderr, "Error: row size of matrix must be 1.\n");
        return false;
    }
    if (matrix->col != 3) {
        fprintf(stderr, "Error: col size of matrix must be 3.\n");
        return false;
    }
    if ((matrix->data[0][0] < 0) || (matrix->data[0][0] > SIZE_MAX)) {
        fprintf(stderr, "Error: n must be positive integer.\n");
        return false;
    }
    if (matrix->data[0][1] >= matrix->data[0][2]) {
        fprintf(stderr, "Error: x0 must be smaller than x1.\n");
        return false;
    }
    return true;
}

int main(void) {
    Matrix *matrix;
    double res;

    if ((matrix = readCsvToMatrix(VECTOR_FILENAME)) == NULL) {
        return EXIT_FAILURE;
    }
    if (!validate(matrix)) {
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }
    // The number of divisions is rounded to unsigned integer.
    res = trapezoid(sq, matrix->data[0][1], matrix->data[0][2],
                    (size_t)matrix->data[0][0]);

    printf("%.7f\n", res);

    freeMatrix(matrix);

    return EXIT_SUCCESS;
}