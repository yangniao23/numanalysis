/*
���l��͗p��{�֐��Q�����t�@�C��
  Dec. 03, 2023     ver 1.3
  Sept. 30, 2021	ver 1.2
�@Jan. 11, 2019		ver 1.1
 */

#include "NAbasic.h"

/*
 csvRead(size_t *row, size_t *col, FILE *fin)
  csv�t�@�C����ǂݍ��ݓ񎟌��z��Ɋi�[����B
        �߂�l: 2�����z��擪�v�f�ւ̃|�C���^�i�����s��j

        �p�����[�^
        �@�o��
                row: �ǂݍ��܂ꂽ�s��
                col: �ǂݍ��܂ꂽ�� (csv�̍ŏ��̃f�[�^�s�Ŋm��)

        �@����
                fin: �ǂݍ��ރt�@�C���̃t�@�C���|�C���^
*/
double **csvRead(size_t *row, size_t *col, FILE *fin) {
    char buf[MAX_STR_LENGTH], buf2[MAX_STR_LENGTH];
    double *mat[MAX_ROW_NUM];
    double **result;

    char *nbuf;
    size_t i = 0, j;
    *row = *col = 0;

    while (fgets(buf, sizeof(buf), fin) != NULL) {
        // #�̓R�����g�s
        if (buf[0] == '#') continue;

        if (i == 0) {
            strcpy(buf2, buf);
            if (strtok(buf2, ",") != NULL) (*col)++;
            while (strtok(NULL, ",\r\n\0") != NULL) (*col)++;
        }

        mat[i] = allocVector(*col);

        mat[i][0] = atof(strtok(buf, ","));
        for (j = 1; j < *col; j++) {
            nbuf = strtok(NULL, ",\r\n\0");
            if (nbuf == NULL)
                mat[i][j] = 0;
            else
                mat[i][j] = atof(nbuf);
        }
        i++;
    }

    // �s�������������̂ŁC���������m�ۂ��C�ꎞ�z�񂩂�R�s�[����
    *row = i;
    if ((result = (double **)malloc(((*row) + 1) * sizeof(double *))) == NULL) {
        fprintf(stderr, "���������m�ۂł��܂��� (from csvRead) \n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < *row; i++) {
        result[i] = mat[i];
    }
    result[*row] = NULL;  // �I�[��NULL�ɂ��Ă���

    // ����ꂽ�񎟌��z��̃|�C���^��Ԃ�
    return result;
}

/*
 allocVector(size_t length)
  vector���m�ۂ���
        �߂�l: 1�����z��擪�v�f�ւ̃|�C���^�i�����s��j

        �p�����[�^
          ����
                length: �x�N�g���̒���
*/
double *allocVector(size_t length) {
    double *vec;

    if ((vec = (double *)malloc(length * sizeof(double))) == NULL) {
        fprintf(stderr, "���������m�ۂł��܂��� (from allocVector) \n");
        exit(EXIT_FAILURE);
    }

    return vec;
}

/*
 allocMatrix(size_t row, size_t col)
  matrix���m�ۂ���
    �߂�l: 2�����z��擪�v�f�ւ̃|�C���^�i�����s��j

        �p�����[�^
          ����
                row: �s��
                col: ��
*/
double **allocMatrix(size_t row, size_t col) {
    double **matrix;
    size_t i;
    if ((matrix = (double **)malloc((row + 1) * sizeof(double *))) == NULL) {
        fprintf(stderr, "���������m�ۂł��܂��� (from allocMatrix) \n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < row; i++) {
        if ((matrix[i] = (double *)malloc(col * sizeof(double))) == NULL) {
            fprintf(stderr, "���������m�ۂł��܂��� (from allocMatrix) \n");
            exit(EXIT_FAILURE);
        }
    }
    matrix[row] = NULL;

    return matrix;
}

/*
  freeMatrix(double **matrix)
        matrix���m�ۂ���
          �߂�l: �J������or���s

          �p�����[�^
                ����
                  matrix: �J���������s��i�ŏI�s�v�f��NULL�j
*/
int freeMatrix(double **matrix) {
    size_t i = 0;
    while (matrix[i] != NULL) {
        free(matrix[i]);
        i++;
    }
    free(matrix);

    return EXIT_SUCCESS;
}

/*
freeVector(double *vector)
  vector���m�ۂ���
          �߂�l: �J������or���s

          �p�����[�^
                ����
                  vector: �J���������x�N�g��
*/
int freeVector(double *vector) {
    free(vector);
    return EXIT_SUCCESS;
}

/*
matrix2colVector(const double **matrix, size_t row, size_t col)
  matrix���x�N�g���ɕϊ�����
          �߂�l: 1�����z��i�x�N�g���j�擪�v�f�ւ̃|�C���^�i�����z��j

          �p�����[�^
                ����
                  matrix: �ϊ��Ώۍs��
                �@row: �ϊ��O�̔z��̍s��
                �@col: �ϊ��O�̔z��̍s��
*/
double *matrix2colVector(const double **matrix, size_t row, size_t col) {
    double *vector;
    size_t i, j, k;

    k = 0;
    vector = allocVector(row * col);
    for (j = 0; j < col; j++) {
        for (i = 0; i < row; i++) {
            vector[k] = matrix[i][j];
            k++;
        }
    }

    return vector;
}

/*
matrix2colVector(const double **matrix, size_t row, size_t col)
  matrix���x�N�g���ɕϊ�����
          �߂�l: �\������

          �p�����[�^
                ����
                  matrix: �o�͑Ώۍs��i�s�ρj
                �@row: �ϊ��O�̔z��̍s��
                �@col: �ϊ��O�̔z��̍s��
*/
int showMatrix(const double **matrix, size_t row, size_t col) {
    size_t i, j;

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            printf("%.4f \t", matrix[i][j]);
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}

/*
tpMatrix(const double **matrix)
  matrix��]�u����
          �߂�l: �]�u�s��

          �p�����[�^
                ����
                  matrix: �]�u�Ώۍs��i�s�ρj
                  row: �]�u�O�̔z��̍s��
                �@col: �]�u�O�̔z��̍s��
*/
double **tpMatrix(const double **matrix, size_t row, size_t col) {
    double **transM = allocMatrix(col, row);
    size_t i, j;

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            transM[j][i] = matrix[i][j];
        }
    }

    return transM;
}

double **copy_matrix(const double **src, double **dst, size_t row, size_t col) {
    size_t i;

    for (i = 0; i < row; i++) {
        memcpy(dst[i], src[i], sizeof(double) * col);
    }
    return dst;
}

int swap_matrix_row(double **matrix, size_t row, size_t i, size_t j) {
    double *tmp;

    if (row <= i || row <= j) {
        fprintf(stderr, "Error: row index is out of range.\n");
        return -1;
    }
    if (i == j) return 0;  // nothing to do

    tmp = matrix[i];
    matrix[i] = matrix[j];
    matrix[j] = tmp;

    return 0;
}
