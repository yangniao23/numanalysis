#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define VECTOR_FILENAME "k11-input.csv"
#define EPS 1e-7

typedef double (*func_t)(double);

void dump_romberg_table(size_t n, size_t p, Vector **T, size_t len) {
    size_t i, j;
    printf("division_num:%zu dimension:%zu\n", n, p);
    puts("Romberg Table");
    for (i = 0; i < len; i++) {
        for (j = 0; j < T[i]->length; j++) {
            printf("%.10f  ", T[i]->data[j]);
        }
        putchar('\n');
    }
}

double romberg(func_t f, double a1, double a2, size_t k_max) {
    size_t i, k, m = 1;
    size_t n = 1, p = 1;  // n = 2^k, p = 4^m
    double result;
    Vector **T;
    bool is_converged = false;

    if ((T = (Vector **)malloc(sizeof(Vector *) * k_max)) == NULL) {
        fprintf(stderr, "Error: malloc() failed.\n");
        return NAN;
    }

    if ((T[0] = allocVector(1)) == NULL) {
        free(T);
        fprintf(stderr, "Error: allocVector() failed.\n");
        return NAN;
    }

    T[0]->data[0] = (a2 - a1) * (f(a1) + f(a2)) / 2.0;

    for (k = 1; ((k < k_max) && !(is_converged)); k++) {
        double h, sum = 0.0;

        if ((T[k] = allocVector(k + 1)) == NULL) {
            for (i = 0; i < k; i++) freeVector(T[i]);
            free(T);
            fprintf(stderr, "Error: allocVector() failed.\n");
            return NAN;
        }

        n *= 2;  // n = 2^k
        h = (a2 - a1) / n;
        // sum = f(a + h) + f(a + 3h) + ... + f(a+(2n-1)h)
        for (i = 1; i < n; i += 2) sum += f(a1 + i * h);
        // T[k][0] = T[k-1][0]/2 + h*sum (k >= 1)
        T[k]->data[0] = T[k - 1]->data[0] / 2 + h * sum;

        for (m = p = 1; m <= k; m++) {
            p *= 2;  // p = 2^m
            // T[k][m] = T[k][m-1] + (T[k][m-1] - T[k-1][m-1])/(p^2-1) (k >= 1)
            T[k]->data[m] =
                T[k]->data[m - 1] +
                (T[k]->data[m - 1] - T[k - 1]->data[m - 1]) / (p * p - 1);
            if (fabs(T[k]->data[m] - T[k - 1]->data[m - 1]) < EPS) {
                is_converged = true;
                T[k]->length = ++m;
                break;
            }
        }
    }

    // dump_romberg_table(n, p, T, k);
    result = T[k - 1]->data[m - 1];
    for (i = 0; i < k; i++) freeVector(T[i]);
    free(T);

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

bool validate(const Matrix *matrix) {
    if (matrix->row != 1) {
        fprintf(stderr, "Error: row size of matrix must be 1.\n");
        return false;
    }
    if (matrix->col != 2) {
        fprintf(stderr, "Error: col size of matrix must be 2.\n");
        return false;
    }
    if (matrix->data[0][0] >= matrix->data[0][1]) {
        fprintf(stderr, "Error: a1 must be smaller than a2.\n");
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

    res = romberg(exp, matrix->data[0][0], matrix->data[0][1], 10);
    printf("result: %f\n", res);

    freeMatrix(matrix);

    return EXIT_SUCCESS;
}