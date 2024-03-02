#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define PARAM_FILENAME "sod.csv"
#define UNUSED(x) (void)(x)

typedef double (*F)(double, double, double);
typedef struct {
    F f;
    F g;
    double x0;
    double y0;
    double width;
    double t0;
    double t_inf;
} Condition;

double f(double x, double y, double t) { return x + 6 * y + t - 10; }
double g(double x, double y, double t) {
    UNUSED(y);
    return x + t - 3;
}

Matrix *solve_sod(Condition *cond) {
    size_t len =
        (size_t)((cond->t_inf - cond->t0) / cond->width) + 1;  // len = n + 1
    Matrix *matrix = allocMatrix(3, len);  // data[0][i] = x_i, data[1][i] = y_i
    size_t i;
    double *x = matrix->data[0];
    double *y = matrix->data[1];
    double *t = matrix->data[2];

    if (matrix == NULL) {
        fprintf(stderr, "Error: allocMatrix() failed.\n");
        return NULL;
    }

    x[0] = cond->x0;
    y[0] = cond->y0;
    t[0] = cond->t0;
    for (i = 1; i < len; i++) {
        x[i] = x[i - 1] + cond->f(x[i - 1], y[i - 1], t[i - 1]) * cond->width;
        y[i] = y[i - 1] + cond->g(x[i - 1], y[i - 1], t[i - 1]) * cond->width;
        t[i] = t[i - 1] + cond->width;
    }

    return matrix;
}

Matrix *solve_sod1(Condition *cond) {
    size_t len =
        (size_t)((cond->t_inf - cond->t0) / cond->width) + 1;  // len = n + 1
    Matrix *matrix = allocMatrix(3, len);  // data[0][i] = x_i, data[1][i] = y_i
    size_t i;
    double *x = matrix->data[0];
    double *y = matrix->data[1];
    double *t = matrix->data[2];

    if (matrix == NULL) {
        fprintf(stderr, "Error: allocMatrix() failed.\n");
        return NULL;
    }

    x[0] = cond->x0;
    y[0] = cond->y0;
    for (i = 1; i < len; i++) {
        double kx[4], ky[4];
        kx[0] = cond->f(x[i - 1], y[i - 1], t[i - 1]);
        ky[0] = cond->g(x[i - 1], y[i - 1], t[i - 1]);
        kx[1] = cond->f(x[i - 1] + cond->width / 2 * kx[0],
                        y[i - 1] + cond->width / 2 * ky[0],
                        t[i - 1] + cond->width / 2);
        ky[1] = cond->g(x[i - 1] + cond->width / 2 * kx[0],
                        y[i - 1] + cond->width / 2 * ky[0],
                        t[i - 1] + cond->width / 2);
        kx[2] = cond->f(x[i - 1] + cond->width / 2 * kx[1],
                        y[i - 1] + cond->width / 2 * ky[1],
                        t[i - 1] + cond->width / 2);
        ky[2] = cond->g(x[i - 1] + cond->width / 2 * kx[1],

                        y[i - 1] + cond->width / 2 * ky[1],
                        t[i - 1] + cond->width / 2);
        kx[3] = cond->f(x[i - 1] + cond->width * kx[2],
                        y[i - 1] + cond->width * ky[2], t[i - 1] + cond->width);
        ky[3] = cond->g(x[i - 1] + cond->width * kx[2],
                        y[i - 1] + cond->width * ky[2], t[i - 1] + cond->width);

        x[i] = x[i - 1] +
               cond->width * (kx[0] + 2 * kx[1] + 2 * kx[2] + kx[3]) / 6;
        y[i] = y[i - 1] +
               cond->width * (ky[0] + 2 * ky[1] + 2 * ky[2] + ky[3]) / 6;
        t[i] = t[i - 1] + cond->width;
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
    // matrix[0] = {x0, y0, width, t0, t_inf}
    if (matrix->row != 1) {
        fprintf(stderr, "Error: row size of matrix must be 1.\n");
        return false;
    }
    if (matrix->col != 5) {
        fprintf(stderr, "Error: col size of matrix must be 3.\n");
        return false;
    }
    if (matrix->data[0][2] < 0) {
        fprintf(stderr, "Error: step width must be positive value.\n");
        return false;
    }
    return true;
}

int main(void) {
    Matrix *matrix, *res;
    size_t i;
    Condition cond;

    if ((matrix = readCsvToMatrix(PARAM_FILENAME)) == NULL) {
        return EXIT_FAILURE;
    }
    if (!validate(matrix)) {
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }
    cond = (Condition){
        .f = f,
        .g = g,
        .x0 = matrix->data[0][0],
        .y0 = matrix->data[0][1],
        .width = matrix->data[0][2],
        .t0 = matrix->data[0][3],
        .t_inf = matrix->data[0][4],
    };
    printf("step width: %f\n", cond.width);
    res = solve_sod(&cond);
    if (res == NULL) {
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }

    for (i = 0; i < res->col; i++) {
        printf("%.6f,%.6f,%.6f\n", res->data[0][i], res->data[1][i],
               res->data[2][i]);
    }

    freeMatrix(res);
    freeMatrix(matrix);

    return EXIT_SUCCESS;
}
