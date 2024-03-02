#include <stdio.h>

#include "NAbasic.h"

int main(void) {
    Matrix *L, *U, *P;
    Matrix *Linv, *Uinv, *res;

    L = allocMatrix(3, 3);
    U = allocMatrix(3, 3);
    P = allocMatrix(3, 3);
    Linv = allocMatrix(3, 3);
    Uinv = allocMatrix(3, 3);

    L->data[0][0] = 1.0;
    L->data[0][1] = 0.0;
    L->data[0][2] = 0.0;
    L->data[1][0] = -0.5;
    L->data[1][1] = 1.0;
    L->data[1][2] = 0.0;
    L->data[2][0] = 0.25;
    L->data[2][1] = -0.5;
    L->data[2][2] = 1.0;

    U->data[0][0] = 8.0;
    U->data[0][1] = 2.0;
    U->data[0][2] = -1.0;
    U->data[1][0] = 0.0;
    U->data[1][1] = 3.0;
    U->data[1][2] = 0.5;
    U->data[2][0] = 0.0;
    U->data[2][1] = 0.0;
    U->data[2][2] = 5.5;

    P->data[0][0] = 0.0;
    P->data[0][1] = 0.0;
    P->data[0][2] = 1.0;
    P->data[1][0] = 0.0;
    P->data[1][1] = 1.0;
    P->data[1][2] = 0.0;
    P->data[2][0] = 1.0;
    P->data[2][1] = 0.0;
    P->data[2][2] = 0.0;

    for (size_t i = 0; i < 3; i++) {
        Linv->data[i][i] = 1.0 / L->data[i][i];
        Uinv->data[i][i] = 1.0 / U->data[i][i];
    }
    Linv->data[1][0] = -L->data[1][0] * Linv->data[0][0] * Linv->data[1][1];
    Linv->data[2][0] =
        (L->data[1][0] * L->data[2][1] * Linv->data[1][1] - L->data[2][0]) *
        Linv->data[0][0] * Linv->data[2][2];
    Linv->data[2][1] = -L->data[2][1] * Linv->data[1][1] * Linv->data[2][2];

    Uinv->data[0][1] = -U->data[0][1] * Uinv->data[0][0] * Uinv->data[1][1];
    Uinv->data[0][2] =
        (U->data[0][1] * U->data[1][2] * Uinv->data[1][1] - U->data[0][2]) *
        Uinv->data[0][0] * Uinv->data[2][2];
    Uinv->data[1][2] = -U->data[1][2] * Uinv->data[1][1] * Uinv->data[2][2];

    showMatrix(Linv);
    showMatrix(Uinv);

    res = matrixProduct(matrixProduct(Uinv, Linv), P);
    showMatrix(res);
    showMatrix(res);
    return 0;
}