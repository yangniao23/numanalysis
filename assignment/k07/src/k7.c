#include "k7.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MATRIX_FILENAME "k7-input_test.csv"
#define LOOP_MAX 100000
#define EPSILON 1e-12

#define NEXT(i) ((i + 1) % 2)
#define CURRENT(i) (i % 2)

int isSquareMatrix(const Matrix *matrix) { return matrix->row == matrix->col; }

static void initVectors(Vector *vectors[], size_t size) {
    size_t i;

    vectors[0] = allocVector(size);
    vectors[1] = allocVector(size);

    // all elements initialized to 1.0 (to calculate all eigenvalues)
    // not to 0.0
    for (i = 1; i < size; i++) {
        vectors[0]->data[i] = 1.0;
        vectors[1]->data[i] = 1.0;
    }
}

static void destroyVectors(Vector *vectors[]) {
    freeVector(vectors[0]);
    freeVector(vectors[1]);
}

static double calculateMaxEigenvalue(const Matrix *matrix, double epsilon) {
    Vector *eigenVectors[2];
    size_t i;
    double eigenvalues[2] = {0.0};

    // matrix->row == matrix->col

    initVectors(eigenVectors, matrix->row);

    i = 0;

    do {
        matrixVectorProduct2(eigenVectors[NEXT(i)], matrix,
                             eigenVectors[CURRENT(i)]);
        eigenvalues[NEXT(i)] =
            innerProduct(eigenVectors[NEXT(i)], eigenVectors[NEXT(i)]) /
            innerProduct(eigenVectors[NEXT(i)], eigenVectors[CURRENT(i)]);
        normalizeVector(eigenVectors[NEXT(i)]);
        i++;
    } while ((fabs((eigenvalues[1] - eigenvalues[0]) /
                   eigenvalues[CURRENT(i)]) > epsilon) &&
             (i < LOOP_MAX));

    destroyVectors(eigenVectors);
    return eigenvalues[CURRENT(i)];
}

static double calculateMinEigenvalue(const Matrix *matrix, double epsilon) {
    Vector *eigenVectors[2];
    size_t i;
    double eigenvalues[2] = {0.0};
    LUMatrices *lu;

    // matrix->row == matrix->col

    if ((lu = LUdecomposition(matrix)) == NULL) {
        fprintf(stderr, "Error: LUdecomposition() failed.\n");
        return NAN;
    }
    initVectors(eigenVectors, matrix->row);
    i = 0;

    do {
        solveSimultaneousByLUMatrix(
            eigenVectors[NEXT(i)], (const Matrix *)matrix,
            (const Vector *)eigenVectors[CURRENT(i)], (const LUMatrices *)lu);
        eigenvalues[NEXT(i)] =
            innerProduct(eigenVectors[NEXT(i)], eigenVectors[CURRENT(i)]) /
            innerProduct(eigenVectors[NEXT(i)], eigenVectors[NEXT(i)]);
        normalizeVector(eigenVectors[NEXT(i)]);
        i++;
    } while ((fabs((eigenvalues[1] - eigenvalues[0]) /
                   eigenvalues[CURRENT(i)]) > epsilon) &&
             (i < LOOP_MAX));

    destroyVectors(eigenVectors);
    freeLUMatrices(lu);
    return eigenvalues[CURRENT(i)];
}

static Matrix *readSquareMatrix(const char *matrix_filename) {
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
    if (!isSquareMatrix(matrix)) {
        printf("Error: only the square matrix is allowed\n");
        freeMatrix(matrix);
        return NULL;
    }

    // row == col
    return matrix;
}

int main(void) {
    Matrix *matrix;
    LUMatrices *lu;

    if ((matrix = readSquareMatrix(MATRIX_FILENAME)) == NULL) {
        return EXIT_FAILURE;
    }

    lu = LUdecomposition(matrix);
    if (lu == NULL) {
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }
    puts("L =");
    showMatrix((const Matrix *)lu->L);
    puts("\nU =");
    showMatrix((const Matrix *)lu->U);
    puts("\nP =");
    showMatrix((const Matrix *)lu->P);
    putchar('\n');
    freeLUMatrices(lu);

    printf("Max eigenvalue: %.4f\n",
           calculateMaxEigenvalue((const Matrix *)matrix, 1e-12));
    printf("Min eigenvalue: %.4f\n",
           calculateMinEigenvalue((const Matrix *)matrix, 1e-12));

    freeMatrix(matrix);

    return EXIT_SUCCESS;
}