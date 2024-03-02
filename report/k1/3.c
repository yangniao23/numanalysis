#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define VECTOR1_FILENAME "k2-input1.csv"
#define VECTOR2_FILENAME "k2-input2.csv"
#define M_PI 3.14159265358979323846

double **calcurate_matrix_product(const double **matrix1, int row1, int col1,
                                  const double **matrix2, int row2, int col2) {
    double **result;
    int i, j, k;

    if (col1 != row2) return NULL;  // invalid matrix size
    result = allocMatrix(row1, col2);
    for (i = 0; i < row1; i++) {
        for (j = 0; j < col2; j++) {
            double tmp = 0.0;
            for (k = 0; k < col1; k++) {
                tmp += matrix1[i][k] * matrix2[k][j];
            }
            result[i][j] = tmp;
        }
    }
    return result;
}

double calcurate_inner_product_by_matrix_product(const double *vector1,
                                                 int vector1_length,
                                                 const double *vector2,
                                                 int vector2_length) {
    double **tmatrix, **result_matrix;
    double result;

    tmatrix = tpMatrix((const double **)&vector2, 1, vector2_length);
    result_matrix =
        calcurate_matrix_product((const double **)&vector1, 1, vector1_length,
                                 (const double **)tmatrix, vector2_length, 1);
    // matrix size is 1x1 because row1 x col2 = 1 x 1
    if (result_matrix == NULL) {
        fprintf(stderr, "could not calcurate inner product\n");
        return 0.0;
    };
    result = result_matrix[0][0];
    freeMatrix(tmatrix);
    return result;
}

double calcurate_vector_angle_by_inner_product(const double *vector1,
                                               int vector1_length,
                                               const double *vector2,
                                               int vector2_length) {
    double inner_product, vector1_square, vector2_square, angle_rad;

    inner_product = calcurate_inner_product_by_matrix_product(
        vector1, vector1_length, vector2, vector2_length);
    vector1_square = calcurate_inner_product_by_matrix_product(
        vector1, vector1_length, vector1, vector1_length);
    vector2_square = calcurate_inner_product_by_matrix_product(
        vector2, vector2_length, vector2, vector2_length);

    angle_rad = acos(inner_product / sqrt(vector1_square * vector2_square));
    return angle_rad * 180.0 / M_PI;
}

int main(void) {
    FILE *vector1_fin, *vector2_fin;
    double **matrix1, **matrix2, *vector1, *vector2, angle;
    int row1, col1, row2, col2;

    if ((vector1_fin = fopen(VECTOR1_FILENAME, "r")) == NULL) {
        fprintf(stderr, "file %s is not found.\n", VECTOR1_FILENAME);
        return EXIT_FAILURE;
    }
    if ((vector2_fin = fopen(VECTOR2_FILENAME, "r")) == NULL) {
        fprintf(stderr, "file %s is not found.\n", VECTOR2_FILENAME);
        return EXIT_FAILURE;
    }

    // Error handlings are processed in csvRead(), so omitted here.
    matrix1 = csvRead(&row1, &col1, vector1_fin);
    matrix2 = csvRead(&row2, &col2, vector2_fin);

    // show warning if either row or column is not 1
    if (((row1 != 1) && (col1 != 1)) || ((row2 != 1) && (col2 != 1))) {
        fprintf(stderr,
                "warning: input contains something that is not a vector. "
                "(row1=%d, col1=%d, row2=%d, col2=%d)\n",
                row1, col1, row2, col2);
    }
    if ((row1 * col1) != (row2 * col2)) {
        fprintf(stderr,
                "input vectors have different length. "
                "(row1=%d, col1=%d, row2=%d, col2=%d)\n",
                row1, col1, row2, col2);
        return EXIT_FAILURE;
    }
    // convert from matrix (anything vector) to column vector
    vector1 = matrix2colVector((const double **)matrix1, row1, col1);
    vector2 = matrix2colVector((const double **)matrix2, row2, col2);
    fclose(vector1_fin);
    fclose(vector2_fin);
    freeMatrix(matrix1);
    freeMatrix(matrix2);

    angle = calcurate_vector_angle_by_inner_product(
        (const double *)vector1, row1 * col1, (const double *)vector2,
        row2 * col2);
    printf("%f\n", angle);

    freeVector(vector1);
    freeVector(vector2);

    return EXIT_SUCCESS;
}