#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "k7.h"

static LUMatrices *initLUdecomposition(const Matrix *matrix) {
    size_t i, j, matrixSize = matrix->row;  // matrix->row == matrix->col
    LUMatrices *lu;

    if ((lu = (LUMatrices *)malloc(sizeof(LUMatrices))) == NULL) {
        fprintf(stderr, "Error: malloc() failed.\n");
        return NULL;
    }

    lu->L = allocMatrix(matrixSize, matrixSize);
    lu->U = copyMatrix(matrix);
    lu->P = allocMatrix(matrixSize, matrixSize);

    // set 0.0 to L and P
    for (i = 0; i < matrixSize; i++) {
        for (j = 0; j < matrixSize; j++) {
            lu->L->data[i][j] = 0.0;
            lu->P->data[i][j] = 0.0;
        }
    }
    // set diagonal elements to 1.0
    for (i = 0; i < matrixSize; i++) {
        lu->L->data[i][i] = lu->P->data[i][i] = 1.0;
    }

    return lu;
}

void freeLUMatrices(LUMatrices *lu) {
    freeMatrix(lu->L);
    freeMatrix(lu->U);
    freeMatrix(lu->P);
}

LUMatrices *LUdecomposition(const Matrix *matrix) {
    size_t i, j, k, matrixSize = matrix->row;
    LUMatrices *lu;

    if (!isSquareMatrix((const Matrix *)matrix)) {
        fprintf(stderr, "Error: matrix is not square matrix.\n");
        return NULL;
    }

    if ((lu = initLUdecomposition(matrix)) == NULL) {
        fprintf(stderr, "Error: initLUdecomposition() failed.\n");
        return NULL;
    }

    for (i = 0; i < matrixSize; i++) {
        size_t max_row = 0;
        double max = 0.0;
        // find maximum absolute value in column i from row i to row end.
        for (j = i; j < matrixSize; j++) {
            if (max < fabs(lu->U->data[j][i])) {
                max = fabs(lu->U->data[j][i]);
                max_row = j;
            }
        }

        swapMatrixRow(lu->U, i, max_row);
        swapMatrixRow(lu->P, i, max_row);
        // swap l_matrix value only calculated (ignore diagonal elements)
        for (j = 0; j < i; j++) {
            double tmp = lu->L->data[i][j];
            lu->L->data[i][j] = lu->L->data[max_row][j];
            lu->L->data[max_row][j] = tmp;
        }

        // forward elimination
        for (j = i + 1; j < matrixSize; j++) {
            double tmp = lu->U->data[j][i] / lu->U->data[i][i];

            for (k = i; k < matrixSize; k++) {
                lu->U->data[j][k] -= lu->U->data[i][k] * tmp;
            }
            lu->L->data[j][i] = tmp;
        }
    }
    return lu;
}

// The result is stored in res.
Vector *solveSimultaneousByLUMatrix(Vector *res, const Matrix *orgMatrix,
                                    const Vector *orgVector,
                                    const LUMatrices *lu) {
    Vector *vector;
    size_t i, j, matrixSize = orgMatrix->row;  // matrix->row == matrix->col

    if (!(isSquareMatrix((const Matrix *)orgMatrix)) ||
        (orgMatrix->row != orgVector->length)) {
        fprintf(stderr, "Error: matrix and vector size are not match.\n");
        return NULL;
    }

    // check if LU decomposition is done
    if ((lu == NULL) || (lu->L == NULL) || (lu->U == NULL) || (lu->P == NULL)) {
        fprintf(stderr, "Error: LU decomposition is not done.\n");
        return NULL;
    }

    vector = allocVector(orgVector->length);

    // solve simultaneous equations
    for (i = 0; i < matrixSize; i++) {
        size_t p_index = 0;
        // get the index with a value 1 in p_matrix
        for (j = 0; j < matrixSize; j++) {
            if (lu->P->data[i][j] == 1.0) {
                p_index = j;
                break;
            }
        }
        vector->data[i] = orgVector->data[p_index];

        for (j = 0; j < i; j++) {
            vector->data[i] -= lu->L->data[i][j] * vector->data[j];
        }
    }

    for (i = 0; i < matrixSize; i++) {
        size_t i_inv = matrixSize - 1 - i;
        res->data[i_inv] = vector->data[i_inv];
        for (j = i_inv + 1; j < matrixSize; j++) {
            res->data[i_inv] -= lu->U->data[i_inv][j] * res->data[j];
        }
        res->data[i_inv] /= lu->U->data[i_inv][i_inv];
    }

    freeVector(vector);
    return res;
}