/*
        ���l��͗p��{�֐��Q�w�b�_�t�@�C��
        Dec. 3, 2023		ver 1.3
        Sept. 30, 2021		ver 1.2
        Oct. 03, 2018		ver 1.0
*/

// �K�v�w�b�_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ��{�p�����[�^��`
#define MAX_STR_LENGTH 200  // 1�s�̕������̍ő�l
#define MAX_ROW_NUM 100     // csv�t�@�C���̍ő�s��

// csv�t�@�C����ǂݍ��ݓ񎟌��z��Ɋi�[����
double **csvRead(size_t *row, size_t *col, FILE *fin);
// vector���m�ۂ���
double *allocVector(size_t length);
// matrix���m�ۂ���
double **allocMatrix(size_t row, size_t col);
// matrix���J������
int freeMatrix(double **matrix);
// vector���J������
int freeVector(double *vector);
// matrix���x�N�g���֕ϊ�����
double *matrix2colVector(const double **matrix, size_t row, size_t col);
// matrix��\������
int showMatrix(const double **matrix, size_t row, size_t col);
// matrix��]�u����
double **tpMatrix(const double **matrix, size_t row, size_t col);
// matrix�̃R�s�[���쐬����
double **copy_matrix(const double **src, double **dst, size_t row, size_t col);
// matrix��i�s�ڂ�j�s�ڂ����ւ���
int swap_matrix_row(double **matrix, size_t row, size_t i, size_t j);