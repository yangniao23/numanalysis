#pragma once

#include "NAbasic.h"

typedef struct {
    Matrix *L;
    Matrix *U;
    Matrix *P;
} LUMatrices;

extern int isSquareMatrix(const Matrix *matrix);
extern LUMatrices *LUdecomposition(const Matrix *matrix);
extern void freeLUMatrices(LUMatrices *lu);
extern Vector *solveSimultaneousByLUMatrix(Vector *res, const Matrix *orgMatrix,
                                           const Vector *orgVector,
                                           const LUMatrices *lu);