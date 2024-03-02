#pragma once
/*
        ���l��͗p��{�֐��Q�w�b�_�t�@�C��
        Dec. 3, 2023		ver 2.0
        Sept. 30, 2021		ver 1.2
        Oct. 03, 2018		ver 1.0
*/

// �K�v�w�b�_
#include <stddef.h>
#include <stdio.h>

// ��{�p�����[�^��`
#define MAX_STR_LENGTH 200  // 1�s�̕������̍ő�l
#define MAX_ROW_NUM 100     // csv�t�@�C���̍ő�s��

// �\���̒�`
typedef struct {
    size_t row;  // �s��
    size_t col;  // ��
    double **data;
} Matrix;

typedef struct {
    size_t size;
    size_t _size;  // Matrix �\���̂Ƃ̌݊����̂���
    double **data;
} SquareMatrix;

typedef struct {
    size_t length;  // �x�N�g���̒���
    double *data;
} Vector;

// csv�t�@�C����ǂݍ��ݓ񎟌��z��Ɋi�[����
Matrix *csvRead(FILE *fin);
// vector���m�ۂ���
Vector *allocVector(size_t length);
// matrix���m�ۂ���
Matrix *allocMatrix(size_t row, size_t col);
// matrix���J������
void freeMatrix(Matrix *matrix);
// vector���J������
void freeVector(Vector *vector);
// matrix���x�N�g���֕ϊ�����
Vector *matrix2colVector(const Matrix *matrix);
// matrix��\������
void showMatrix(const Matrix *matrix);
// matrix��]�u����
Matrix *tpMatrix(const Matrix *matrix);
// matrix�̃R�s�[���쐬����
Matrix *copyMatrix(const Matrix *src);
// matrix��i�s�ڂ�j�s�ڂ����ւ��� (����p����)
int swapMatrixRow(Matrix *matrix, size_t i, size_t j);
// vector1��vector2�̓��ς��v�Z����
double innerProduct(const Vector *vector1, const Vector *vector2);
// vector�̃m�������v�Z����
double vectorNorm(const Vector *vector);
// vector�𐳋K������ (����p����)
void normalizeVector(Vector *vector);
// matrix1��matrix2�̐ς��v�Z���� (�V����matrix���쐬)
Matrix *matrixProduct(const Matrix *matrix1, const Matrix *matrix2);
// matrix1��matrix2�̐ς��v�Z���� (res�Ɍ��ʂ��i�[)
Matrix *matrixProduct2(Matrix *res, const Matrix *matrix1,
                       const Matrix *matrix2);
// matrix �� vector �̐ς��v�Z���� (�V����vector���쐬)
Vector *matrixVectorProduct(const Matrix *matrix, const Vector *vector);
// matrix �� vector �̐ς��v�Z���� (res�Ɍ��ʂ��i�[)
Vector *matrixVectorProduct2(Vector *res, const Matrix *matrix,
                             const Vector *vector);
// vector ��\������
void showVector(const Vector *vector);
// SquareMatrix���m�ۂ���
SquareMatrix *allocSquareMatrix(size_t size);
// y = Ax ������
Vector *solveLinearEquation(const SquareMatrix *org_A, const Vector *org_y);
// Vector �� i �Ԗڂ� j �Ԗڂ����ւ��� (����p����)
int swapVectorElement(Vector *vector, size_t i, size_t j);