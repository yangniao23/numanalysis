/*
���l��͗p��{�֐��Q�����t�@�C��
  Dec. 5, 2023     ver 1.3
  Dec. 3, 2023    ver 1.3
  Sept. 30, 2021	ver 1.2
�@Jan. 11, 2019		ver 1.1
 */
#include "NAbasic.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 csvRead(FILE *fin)
    csv�t�@�C����ǂݍ��� Matrix �\���̂Ɋi�[����B
        �߂�l: Matrix �\���̂ւ̃|�C���^�i�����s��j

        �p�����[�^
            ����
                fin: �ǂݍ��ރt�@�C���̃t�@�C���|�C���^
*/
Matrix *csvRead(FILE *fin) {
    char buf[MAX_STR_LENGTH], buf2[MAX_STR_LENGTH];
    double *mat[MAX_ROW_NUM];
    Matrix *result;

    char *nbuf;
    size_t row = 0, col = 0, i = 0, j;

    while (fgets(buf, sizeof(buf), fin) != NULL) {
        // #�̓R�����g�s
        if (buf[0] == '#') continue;

        if (i == 0) {
            strcpy(buf2, buf);
            if (strtok(buf2, ",") != NULL) col++;
            while (strtok(NULL, ",\r\n\0") != NULL) col++;
        }

        if ((mat[i] = (double *)malloc(col * sizeof(double))) == NULL) {
            fprintf(stderr, "���������m�ۂł��܂��� (from csvRead) \n");
            for (j = 0; j < i; j++) free(mat[j]);
            return NULL;
        }

        mat[i][0] = atof(strtok(buf, ","));
        for (j = 1; j < col; j++) {
            nbuf = strtok(NULL, ",\r\n\0");
            if (nbuf == NULL)
                mat[i][j] = 0;
            else
                mat[i][j] = atof(nbuf);
        }
        i++;
    }

    // �s�������������̂ŁC���������m�ۂ��C�ꎞ�z�񂩂�R�s�[����
    row = i;
    if ((result = allocMatrix(row, col)) == NULL) {
        fprintf(stderr, "���������m�ۂł��܂��� (from csvRead) \n");
        for (j = 0; j < row; j++) free(mat[j]);
        return NULL;
    }
    for (i = 0; i < row; i++) {
        result->data[i] = mat[i];
    }
    // ����ꂽ Matrix �\���̂̃|�C���^��Ԃ�
    return result;
}

/*
 allocVector(size_t length)
    vector���m�ۂ���
        �߂�l: Vector �\���̂ւ̃|�C���^�i�����s��j

        �p�����[�^
          ����
                length: �x�N�g���̒���
*/
Vector *allocVector(size_t length) {
    Vector *vec;

    if ((vec = (Vector *)malloc(sizeof(Vector))) == NULL) {
        fprintf(stderr, "���������m�ۂł��܂��� (from allocVector) \n");
        return NULL;
    }
    vec->length = length;
    if ((vec->data = (double *)malloc(length * sizeof(double))) == NULL) {
        fprintf(stderr, "���������m�ۂł��܂��� (from allocVector) \n");
        free(vec);
        return NULL;
    }

    return vec;
}

/*
 allocMatrix(size_t row, size_t col)
  matrix���m�ۂ���
    �߂�l: Matrix �\���̂ւ̃|�C���^�i�����s��j

        �p�����[�^
            ����
                row: �s��
                col: ��
*/
Matrix *allocMatrix(size_t row, size_t col) {
    Matrix *matrix;
    size_t i;

    if ((matrix = (Matrix *)malloc(sizeof(Matrix))) == NULL) {
        fprintf(stderr, "���������m�ۂł��܂��� (from allocMatrix) \n");
        return NULL;
    }
    matrix->row = row;
    matrix->col = col;
    if ((matrix->data = (double **)malloc(row * sizeof(double *))) == NULL) {
        fprintf(stderr, "���������m�ۂł��܂��� (from allocMatrix) \n");
        free(matrix);
        return NULL;
    }
    for (i = 0; i < row; i++) {
        if ((matrix->data[i] = (double *)malloc(col * sizeof(double))) ==
            NULL) {
            size_t j;
            fprintf(stderr, "���������m�ۂł��܂��� (from allocMatrix) \n");
            for (j = 0; j < i; j++) free(matrix->data[j]);
            free(matrix);
            return NULL;
        }
    }

    return matrix;
}

/*
  freeMatrix(Matrix *matrix)
    matrix���m�ۂ���
          �߂�l: �Ȃ�

          �p�����[�^
                ����
                    matrix: �J���������s��i�ŏI�s�v�f��NULL�j
*/
void freeMatrix(Matrix *matrix) {
    size_t i = 0;

    for (i = 0; i < matrix->row; i++) free(matrix->data[i]);
    free(matrix->data);
    free(matrix);
}

/*
freeVector(Vector *vector)
  vector���m�ۂ���
          �߂�l: �Ȃ�

          �p�����[�^
                ����
                  vector: �J���������x�N�g��
*/
void freeVector(Vector *vector) {
    free(vector->data);
    free(vector);
}

/*
matrix2colVector(const double **matrix, size_t row, size_t col)
    matrix���x�N�g���ɕϊ�����
          �߂�l: 1�����z��i�x�N�g���j�擪�v�f�ւ̃|�C���^�i�����z��j

          �p�����[�^
                ����
                    matrix: �ϊ��Ώۍs��
                    row: �ϊ��O�̔z��̍s��
                    col: �ϊ��O�̔z��̍s��
*/
Vector *matrix2colVector(const Matrix *matrix) {
    Vector *vector;
    size_t i, j, k;

    k = 0;
    vector = allocVector(matrix->row * matrix->col);
    for (j = 0; j < matrix->col; j++) {
        for (i = 0; i < matrix->row; i++) {
            vector->data[k] = matrix->data[i][j];
            k++;
        }
    }

    return vector;
}

/*
showMatrix(const Matrix *matrix, size_t row, size_t col)
  matrix��W���o�͂ɕ\������
          �߂�l: �Ȃ�

          �p�����[�^
                ����
                    matrix: �o�͑Ώۍs��i�s�ρj
*/

void showMatrix(const Matrix *matrix) {
    size_t i, j;

    for (i = 0; i < matrix->row; i++) {
        for (j = 0; j < matrix->col; j++) {
            if (matrix->data[i][j] < 0) {
                printf("%.4f  ", matrix->data[i][j]);
            } else {
                printf(" %.4f  ", matrix->data[i][j]);
            }
        }
        printf("\n");
    }
}

/*
tpMatrix(const Matrix *matrix)
  matrix��]�u����
          �߂�l: �]�u�s��

          �p�����[�^
                ����
                  matrix: �]�u�Ώۍs��i�s�ρj
*/

Matrix *tpMatrix(const Matrix *matrix) {
    Matrix *transM = allocMatrix(matrix->col, matrix->row);
    size_t i, j;

    for (i = 0; i < matrix->row; i++) {
        for (j = 0; j < matrix->col; j++) {
            transM->data[j][i] = matrix->data[i][j];
        }
    }

    return transM;
}

/*
copyMatrix(double **dst, const Matrix **src)
  matrix�̃R�s�[���쐬����
          �߂�l: �R�s�[�s��

          �p�����[�^
                ����
                    src: �R�s�[���s��i�s�ρj
*/

Matrix *copyMatrix(const Matrix *src) {
    Matrix *res = allocMatrix(src->row, src->col);
    size_t i;

    for (i = 0; i < src->row; i++) {
        memcpy(res->data[i], src->data[i], sizeof(double) * src->col);
    }
    return res;
}

/*
swapMatrixRow(Matrix *matrix, size_t i, size_t j)
  matrix��i�s�ڂ�j�s�ڂ����ւ���
          �߂�l: 0: ����I��, -1: �ُ�I��

          �p�����[�^
                ����
                    matrix: ����ւ��Ώۍs��
                    i: ����ւ��Ώۍs�ԍ�
                    j: ����ւ��Ώۍs�ԍ�
*/

int swapMatrixRow(Matrix *matrix, size_t i, size_t j) {
    double *tmp;

    if (matrix->row <= i || matrix->row <= j) {
        fprintf(stderr, "Error: row index is out of range\n");
        return -1;
    }
    if (i == j) return 0;  // nothing to do

    tmp = matrix->data[i];
    matrix->data[i] = matrix->data[j];
    matrix->data[j] = tmp;

    return 0;
}

/*
innerProduct(const Vector *vector1, const Vector *vector2)
  vector1��vector2�̓��ς��v�Z����
          �߂�l: ����

          �p�����[�^
                ����
                    vector1: ���όv�Z�Ώۃx�N�g��
                    vector2: ���όv�Z�Ώۃx�N�g��
*/

double innerProduct(const Vector *vector1, const Vector *vector2) {
    size_t i;
    double res = 0.0;

    if (vector1->length != vector2->length) {
        fprintf(stderr, "Error: vector length is not equal\n");
        return NAN;
    }

    for (i = 0; i < vector1->length; i++) {
        res += vector1->data[i] * vector2->data[i];
    }

    return res;
}

/*
vectorNorm(const Vector *vector)
  vector�̃m�������v�Z����
          �߂�l: �m����

          �p�����[�^
                ����
                    vector: �m�����v�Z�Ώۃx�N�g��
*/

double vectorNorm(const Vector *vector) {
    double res = innerProduct(vector, vector);

    return sqrt(res);
}

/*
normalizeVector(Vector *vector)
  vector�𐳋K������
          �߂�l: �Ȃ�

          �p�����[�^
                ����
                    vector: ���K���Ώۃx�N�g��
*/

void normalizeVector(Vector *vector) {
    size_t i;
    double norm = vectorNorm(vector);

    for (i = 0; i < vector->length; i++) {
        vector->data[i] = vector->data[i] / norm;
    }
}

/*
matrixProduct(const Matrix *matrix1, const Matrix *matrix2)
  matrix1��matrix2�̐ς��v�Z����
          �߂�l: �ύs��

          �p�����[�^
                ����
                    matrix1: �όv�Z�Ώۍs��
                    matrix2: �όv�Z�Ώۍs��
*/

Matrix *matrixProduct(const Matrix *matrix1, const Matrix *matrix2) {
    Matrix *res = allocMatrix(matrix1->row, matrix2->col);

    if (matrixProduct2(res, matrix1, matrix2) == NULL) {
        freeMatrix(res);
        return NULL;
    }

    return res;
}

/*
matrixProduct2(Matrix *res, const Matrix *matrix1, const Matrix *matrix2)
  matrix1��matrix2�̐ς��v�Z����
          �߂�l: �ύs��

          �p�����[�^
                ����
                    res: �ύs����i�[����s��
                    matrix1: �όv�Z�Ώۍs��
                    matrix2: �όv�Z�Ώۍs��
*/

Matrix *matrixProduct2(Matrix *res, const Matrix *matrix1,
                       const Matrix *matrix2) {
    size_t i, j, k;

    if ((matrix1->col != matrix2->row) || (res->row != matrix1->row) ||
        (res->col != matrix2->col)) {
        fprintf(stderr, "Error: matrix size is not equal\n");
        return NULL;
    }

    for (i = 0; i < matrix1->row; i++) {
        for (j = 0; j < matrix2->col; j++) {
            double tmp = 0.0;
            for (k = 0; k < matrix1->col; k++) {
                tmp += matrix1->data[i][k] * matrix2->data[k][j];
            }
            res->data[i][j] = tmp;
        }
    }

    return res;
}

/*
matrixVectorProduct(const Matrix *matrix, const Vector *vector)
  matrix �� vector �̐ς��v�Z����
          �߂�l: �σx�N�g��

          �p�����[�^
                ����
                    matrix: �όv�Z�Ώۍs��
                    vector: �όv�Z�Ώۃx�N�g��
*/

Vector *matrixVectorProduct(const Matrix *matrix, const Vector *vector) {
    Vector *res = allocVector(matrix->row);

    if (matrixVectorProduct2(res, matrix, vector) == NULL) {
        freeVector(res);
        return NULL;
    }

    return res;
}

/*
matrixVectorProduct2(Vector *res, const Matrix *matrix, const Vector *vector)
  matrix �� vector �̐ς��v�Z����
          �߂�l: �σx�N�g��

          �p�����[�^
                ����
                    res: �σx�N�g�����i�[����x�N�g��
                    matrix: �όv�Z�Ώۍs��
                    vector: �όv�Z�Ώۃx�N�g��
*/

Vector *matrixVectorProduct2(Vector *res, const Matrix *matrix,
                             const Vector *vector) {
    size_t i, j;

    if ((matrix->col != vector->length) || (res->length != matrix->row)) {
        fprintf(stderr, "Error: matrix size is not equal\n");
        return NULL;
    }

    for (i = 0; i < matrix->row; i++) {
        double tmp = 0.0;
        for (j = 0; j < matrix->col; j++) {
            tmp += matrix->data[i][j] * vector->data[j];
        }
        res->data[i] = tmp;
    }

    return res;
}

/*
showVector(const Vector *vector)
  vector��W���o�͂ɕ\������
          �߂�l: �Ȃ�

          �p�����[�^
                ����
                    vector: �o�͑Ώۃx�N�g���i�s�ρj
*/

void showVector(const Vector *vector) {
    size_t i;

    for (i = 0; i < vector->length; i++) {
        printf("%.4f \t", vector->data[i]);
    }
    printf("\n");
}

/*
allocSquareMatrix(size_t size)
  SquareMatrix���m�ۂ���
          �߂�l: SquareMatrix �\���̂ւ̃|�C���^�i�����s��j

          �p�����[�^
                ����
                    size: �s��̃T�C�Y
*/

SquareMatrix *allocSquareMatrix(size_t size) {
    SquareMatrix *matrix;

    if ((matrix = (SquareMatrix *)allocMatrix(size, size)) == NULL) {
        fprintf(stderr, "���������m�ۂł��܂��� (from allocSquareMatrix) \n");
        return NULL;
    }

    return matrix;
}

/*
solveLinearEquation(SquareMatrix *A, Vector *b)
  y = Ax ������
          �߂�l: ���x�N�g��

          �p�����[�^
                ����
                    A: �W���s��
                    b: �萔�x�N�g��
*/

Vector *solveLinearEquation(const SquareMatrix *org_A, const Vector *org_y) {
    SquareMatrix *A;
    Vector *y;
    Vector *x;
    size_t i, j, k;

    // �T�C�Y�m�F
    if (org_A->size != org_y->length) {
        fprintf(stderr, "Error: matrix size is not equal\n");
        return NULL;
    }

    // A �� y �փR�s�[
    A = (SquareMatrix *)copyMatrix((Matrix *)org_A);
    y = allocVector(org_y->length);
    for (i = 0; i < y->length; i++) {
        y->data[i] = org_y->data[i];
    }
    x = allocVector(y->length);

    for (i = 0; i < A->size; i++) {
        size_t max_row = 0;
        double max = 0.0;
        // ��΍ő�l�����s��T��
        for (j = i; j < A->size; j++) {
            if (fabs(A->data[j][i]) > max) {
                max = fabs(A->data[j][i]);
                max_row = j;
            }
        }

        swapMatrixRow((Matrix *)A, i, max_row);
        swapVectorElement(y, i, max_row);

        // �O�i����
        for (j = i + 1; j < A->size; j++) {
            double tmp = A->data[j][i] / A->data[i][i];

            for (k = i; k < A->size; k++) {
                A->data[j][k] -= A->data[i][k] * tmp;
            }
            y->data[j] -= y->data[i] * tmp;
        }
    }

    // ��ޑ��
    for (i = A->size; i > 0; i--) {
        double row_value = y->data[i - 1];

        for (j = A->size; j > i; j--) {
            row_value -= A->data[i - 1][j - 1] * x->data[j - 1];
        }
        x->data[i - 1] = row_value / A->data[i - 1][i - 1];
    }
    freeMatrix((Matrix *)A);
    freeVector(y);

    return x;
}

/*
swapVectorElement(Vector *vector, size_t i, size_t j)
  Vector �� i �Ԗڂ� j �Ԗڂ����ւ���
          �߂�l: 0: ����I��, -1: �ُ�I��

          �p�����[�^
                ����
                    vector: ����ւ��Ώۃx�N�g��
                    i: ����ւ��Ώۗv�f�ԍ�
                    j: ����ւ��Ώۗv�f�ԍ�
*/

int swapVectorElement(Vector *vector, size_t i, size_t j) {
    double tmp;

    if (vector->length <= i || vector->length <= j) {
        fprintf(stderr, "Error: vector index is out of range\n");
        return -1;
    }
    if (i == j) return 0;  // nothing to do

    tmp = vector->data[i];
    vector->data[i] = vector->data[j];
    vector->data[j] = tmp;

    return 0;
}

/*
resizeMatrix(Matrix *matrix, size_t row, size_t col)
  Matrix �̃T�C�Y��ύX����
          �߂�l: Matrix �\���̂ւ̃|�C���^

          �p�����[�^
                ����
                    matrix: �T�C�Y�ύX�Ώۍs��
                    row: �ύX��̍s��
                    col: �ύX��̗�
*/

Matrix *resizeMatrix(Matrix *matrix, size_t row, size_t col) {
    Matrix *res;
    size_t i;

    if ((res = allocMatrix(row, col)) == NULL) {
        fprintf(stderr, "���������m�ۂł��܂��� (from resizeMatrix) \n");
        return NULL;
    }

    fillMatrix(res, 0.0);

    for (i = 0; i < MIN(matrix->row, row); i++) {
        size_t length = MIN(matrix->col, col);

        memcpy(res->data[i], matrix->data[i], sizeof(double) * length);
    }

    freeMatrix(matrix);

    return res;
}

/*
resizeSquareMatrix(SquareMatrix *matrix, size_t size)
  SquareMatrix �̃T�C�Y��ύX����
          �߂�l: SquareMatrix �\���̂ւ̃|�C���^

          �p�����[�^
                ����
                    matrix: �T�C�Y�ύX�Ώۍs��
                    size: �ύX��̃T�C�Y
*/

SquareMatrix *resizeSquareMatrix(SquareMatrix *matrix, size_t size) {
    SquareMatrix *res;

    if ((res = (SquareMatrix *)resizeMatrix((Matrix *)matrix, size, size)) ==
        NULL) {
        fprintf(stderr, "���������m�ۂł��܂��� (from resizeSquareMatrix) \n");
        return NULL;
    }

    return res;
}

/*
fillMatrix(Matrix *matrix, double value)
  Matrix �� value �ŏ���������
          �߂�l: �Ȃ�

          �p�����[�^
                ����
                    matrix: �������Ώۍs��
                    value: �������l
*/

void fillMatrix(Matrix *matrix, double value) {
    size_t i, j;

    for (i = 0; i < matrix->row; i++) {
        for (j = 0; j < matrix->col; j++) {
            matrix->data[i][j] = value;
        }
    }
}