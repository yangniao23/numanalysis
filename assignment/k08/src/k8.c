#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define COORDINATES_FILENAME "k8-input.csv"
#define DEGREE 2

double power(double x, size_t n) {
    double result = 1.0;
    size_t i;

    for (i = 0; i < n; i++) {
        result *= x;
    }

    return result;
}

Vector *LeastSquaresApproximation(const Matrix *coordinates, size_t degree) {
    Vector *vector = allocVector(degree + 1);
    SquareMatrix *matrix = allocSquareMatrix(degree + 1);
    size_t i, j, k;

    for (i = 0; i < vector->length; i++) {
        for (j = 0; j < coordinates->row; j++) {
            vector->data[i] +=
                coordinates->data[j][1] * power(coordinates->data[j][0], i);
        }
    }

    for (i = 0; i < matrix->size; i++) {
        for (j = i; j < matrix->size; j++) {
            for (k = 0; k < coordinates->row; k++) {
                matrix->data[i][j] += power(coordinates->data[k][0], i + j);
            }
            matrix->data[j][i] = matrix->data[i][j];
        }
    }

    return solveLinearEquation(matrix, vector);
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

void plotGraph(const Matrix *coordinates, const Vector *vector) {
    FILE *pOut;
    size_t i;

    if ((pOut = popen("gnuplot", "w")) == NULL) {
        fprintf(stderr, "Error: gnuplot is not found.\n");
        return;
    }

    fprintf(pOut, "set zeroaxis\n");
    fprintf(pOut, "set xrange [%f:%f]\n", coordinates->data[0][0] - 0.5,
            coordinates->data[coordinates->row - 1][0] + 0.5);

    fprintf(pOut, "plot ");
    // plot approximation
    for (i = vector->length - 1; i > 0; i--)
        fprintf(pOut, "%2.2f * x**%ld + ", vector->data[i], i);
    fprintf(pOut, "%2.2f lw 2, ", vector->data[0]);
    // plot points
    fprintf(pOut, "'-' w p lw 2 title 'points'\n");
    for (i = 0; i < coordinates->row; i++)
        fprintf(pOut, "%f\t%f\n", coordinates->data[i][0],
                coordinates->data[i][1]);
    fprintf(pOut, "\ne\n");

    fflush(pOut);
    printf("press enter key to terminate program\n");
    getchar();
    pclose(pOut);
}

int main(void) {
    Matrix *coordinates;
    Vector *vector;

    if ((coordinates = readCsvToMatrix(COORDINATES_FILENAME)) == NULL) {
        return EXIT_FAILURE;
    }

    if ((vector = LeastSquaresApproximation(coordinates, DEGREE)) == NULL) {
        freeMatrix(coordinates);
        return EXIT_FAILURE;
    }

    showVector(vector);

    plotGraph(coordinates, vector);

    freeMatrix(coordinates);
    freeVector(vector);

    return EXIT_SUCCESS;
}