/*
数値解析用基本関数群実装ファイル
  Dec. 03, 2023     ver 1.3
  Sept. 30, 2021	ver 1.2
　Jan. 11, 2019		ver 1.1
 */

#include "NAbasic.h"

/*
 csvRead(size_t *row, size_t *col, FILE *fin)
  csvファイルを読み込み二次元配列に格納する。
        戻り値: 2次元配列先頭要素へのポインタ（実数行列）

        パラメータ
        　出力
                row: 読み込まれた行数
                col: 読み込まれた列数 (csvの最初のデータ行で確定)

        　入力
                fin: 読み込むファイルのファイルポインタ
*/
double **csvRead(size_t *row, size_t *col, FILE *fin) {
    char buf[MAX_STR_LENGTH], buf2[MAX_STR_LENGTH];
    double *mat[MAX_ROW_NUM];
    double **result;

    char *nbuf;
    size_t i = 0, j;
    *row = *col = 0;

    while (fgets(buf, sizeof(buf), fin) != NULL) {
        // #はコメント行
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

    // 行数が分かったので，メモリを確保し，一時配列からコピーする
    *row = i;
    if ((result = (double **)malloc(((*row) + 1) * sizeof(double *))) == NULL) {
        fprintf(stderr, "メモリを確保できません (from csvRead) \n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < *row; i++) {
        result[i] = mat[i];
    }
    result[*row] = NULL;  // 終端をNULLにしておく

    // 得られた二次元配列のポインタを返す
    return result;
}

/*
 allocVector(size_t length)
  vectorを確保する
        戻り値: 1次元配列先頭要素へのポインタ（実数行列）

        パラメータ
          入力
                length: ベクトルの長さ
*/
double *allocVector(size_t length) {
    double *vec;

    if ((vec = (double *)malloc(length * sizeof(double))) == NULL) {
        fprintf(stderr, "メモリを確保できません (from allocVector) \n");
        exit(EXIT_FAILURE);
    }

    return vec;
}

/*
 allocMatrix(size_t row, size_t col)
  matrixを確保する
    戻り値: 2次元配列先頭要素へのポインタ（実数行列）

        パラメータ
          入力
                row: 行数
                col: 列数
*/
double **allocMatrix(size_t row, size_t col) {
    double **matrix;
    size_t i;
    if ((matrix = (double **)malloc((row + 1) * sizeof(double *))) == NULL) {
        fprintf(stderr, "メモリを確保できません (from allocMatrix) \n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < row; i++) {
        if ((matrix[i] = (double *)malloc(col * sizeof(double))) == NULL) {
            fprintf(stderr, "メモリを確保できません (from allocMatrix) \n");
            exit(EXIT_FAILURE);
        }
    }
    matrix[row] = NULL;

    return matrix;
}

/*
  freeMatrix(double **matrix)
        matrixを確保する
          戻り値: 開放成功or失敗

          パラメータ
                入力
                  matrix: 開放したい行列（最終行要素はNULL）
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
  vectorを確保する
          戻り値: 開放成功or失敗

          パラメータ
                入力
                  vector: 開放したいベクトル
*/
int freeVector(double *vector) {
    free(vector);
    return EXIT_SUCCESS;
}

/*
matrix2colVector(const double **matrix, size_t row, size_t col)
  matrixを列ベクトルに変換する
          戻り値: 1次元配列（ベクトル）先頭要素へのポインタ（実数配列）

          パラメータ
                入力
                  matrix: 変換対象行列
                　row: 変換前の配列の行数
                　col: 変換前の配列の行数
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
  matrixを列ベクトルに変換する
          戻り値: 表示結果

          パラメータ
                入力
                  matrix: 出力対象行列（不変）
                　row: 変換前の配列の行数
                　col: 変換前の配列の行数
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
  matrixを転置する
          戻り値: 転置行列

          パラメータ
                入力
                  matrix: 転置対象行列（不変）
                  row: 転置前の配列の行数
                　col: 転置前の配列の行数
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
