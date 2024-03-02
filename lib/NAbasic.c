/*
数値解析用基本関数群実装ファイル
  Dec. 5, 2023     ver 1.3
  Dec. 3, 2023    ver 1.3
  Sept. 30, 2021	ver 1.2
　Jan. 11, 2019		ver 1.1
 */
#include "NAbasic.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 csvRead(FILE *fin)
    csvファイルを読み込み Matrix 構造体に格納する。
        戻り値: Matrix 構造体へのポインタ（実数行列）

        パラメータ
            入力
                fin: 読み込むファイルのファイルポインタ
*/
Matrix *csvRead(FILE *fin) {
    char buf[MAX_STR_LENGTH], buf2[MAX_STR_LENGTH];
    double *mat[MAX_ROW_NUM];
    Matrix *result;

    char *nbuf;
    size_t row = 0, col = 0, i = 0, j;

    while (fgets(buf, sizeof(buf), fin) != NULL) {
        // #はコメント行
        if (buf[0] == '#') continue;

        if (i == 0) {
            strcpy(buf2, buf);
            if (strtok(buf2, ",") != NULL) col++;
            while (strtok(NULL, ",\r\n\0") != NULL) col++;
        }

        if ((mat[i] = (double *)malloc(col * sizeof(double))) == NULL) {
            fprintf(stderr, "メモリを確保できません (from csvRead) \n");
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

    // 行数が分かったので，メモリを確保し，一時配列からコピーする
    row = i;
    if ((result = allocMatrix(row, col)) == NULL) {
        fprintf(stderr, "メモリを確保できません (from csvRead) \n");
        for (j = 0; j < row; j++) free(mat[j]);
        return NULL;
    }
    for (i = 0; i < row; i++) {
        result->data[i] = mat[i];
    }
    // 得られた Matrix 構造体のポインタを返す
    return result;
}

/*
 allocVector(size_t length)
    vectorを確保する
        戻り値: Vector 構造体へのポインタ（実数行列）

        パラメータ
          入力
                length: ベクトルの長さ
*/
Vector *allocVector(size_t length) {
    Vector *vec;

    if ((vec = (Vector *)malloc(sizeof(Vector))) == NULL) {
        fprintf(stderr, "メモリを確保できません (from allocVector) \n");
        return NULL;
    }
    vec->length = length;
    if ((vec->data = (double *)malloc(length * sizeof(double))) == NULL) {
        fprintf(stderr, "メモリを確保できません (from allocVector) \n");
        free(vec);
        return NULL;
    }

    return vec;
}

/*
 allocMatrix(size_t row, size_t col)
  matrixを確保する
    戻り値: Matrix 構造体へのポインタ（実数行列）

        パラメータ
            入力
                row: 行数
                col: 列数
*/
Matrix *allocMatrix(size_t row, size_t col) {
    Matrix *matrix;
    size_t i;

    if ((matrix = (Matrix *)malloc(sizeof(Matrix))) == NULL) {
        fprintf(stderr, "メモリを確保できません (from allocMatrix) \n");
        return NULL;
    }
    matrix->row = row;
    matrix->col = col;
    if ((matrix->data = (double **)malloc(row * sizeof(double *))) == NULL) {
        fprintf(stderr, "メモリを確保できません (from allocMatrix) \n");
        free(matrix);
        return NULL;
    }
    for (i = 0; i < row; i++) {
        if ((matrix->data[i] = (double *)malloc(col * sizeof(double))) ==
            NULL) {
            size_t j;
            fprintf(stderr, "メモリを確保できません (from allocMatrix) \n");
            for (j = 0; j < i; j++) free(matrix->data[j]);
            free(matrix);
            return NULL;
        }
    }

    return matrix;
}

/*
  freeMatrix(Matrix *matrix)
    matrixを確保する
          戻り値: なし

          パラメータ
                入力
                    matrix: 開放したい行列（最終行要素はNULL）
*/
void freeMatrix(Matrix *matrix) {
    size_t i = 0;

    for (i = 0; i < matrix->row; i++) free(matrix->data[i]);
    free(matrix->data);
    free(matrix);
}

/*
freeVector(Vector *vector)
  vectorを確保する
          戻り値: なし

          パラメータ
                入力
                  vector: 開放したいベクトル
*/
void freeVector(Vector *vector) {
    free(vector->data);
    free(vector);
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
  matrixを標準出力に表示する
          戻り値: なし

          パラメータ
                入力
                    matrix: 出力対象行列（不変）
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
  matrixを転置する
          戻り値: 転置行列

          パラメータ
                入力
                  matrix: 転置対象行列（不変）
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
  matrixのコピーを作成する
          戻り値: コピー行列

          パラメータ
                入力
                    src: コピー元行列（不変）
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
  matrixのi行目とj行目を入れ替える
          戻り値: 0: 正常終了, -1: 異常終了

          パラメータ
                入力
                    matrix: 入れ替え対象行列
                    i: 入れ替え対象行番号
                    j: 入れ替え対象行番号
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
  vector1とvector2の内積を計算する
          戻り値: 内積

          パラメータ
                入力
                    vector1: 内積計算対象ベクトル
                    vector2: 内積計算対象ベクトル
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
  vectorのノルムを計算する
          戻り値: ノルム

          パラメータ
                入力
                    vector: ノルム計算対象ベクトル
*/

double vectorNorm(const Vector *vector) {
    double res = innerProduct(vector, vector);

    return sqrt(res);
}

/*
normalizeVector(Vector *vector)
  vectorを正規化する
          戻り値: なし

          パラメータ
                入力
                    vector: 正規化対象ベクトル
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
  matrix1とmatrix2の積を計算する
          戻り値: 積行列

          パラメータ
                入力
                    matrix1: 積計算対象行列
                    matrix2: 積計算対象行列
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
  matrix1とmatrix2の積を計算する
          戻り値: 積行列

          パラメータ
                入力
                    res: 積行列を格納する行列
                    matrix1: 積計算対象行列
                    matrix2: 積計算対象行列
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
  matrix と vector の積を計算する
          戻り値: 積ベクトル

          パラメータ
                入力
                    matrix: 積計算対象行列
                    vector: 積計算対象ベクトル
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
  matrix と vector の積を計算する
          戻り値: 積ベクトル

          パラメータ
                入力
                    res: 積ベクトルを格納するベクトル
                    matrix: 積計算対象行列
                    vector: 積計算対象ベクトル
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
  vectorを標準出力に表示する
          戻り値: なし

          パラメータ
                入力
                    vector: 出力対象ベクトル（不変）
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
  SquareMatrixを確保する
          戻り値: SquareMatrix 構造体へのポインタ（実数行列）

          パラメータ
                入力
                    size: 行列のサイズ
*/

SquareMatrix *allocSquareMatrix(size_t size) {
    SquareMatrix *matrix;

    if ((matrix = (SquareMatrix *)allocMatrix(size, size)) == NULL) {
        fprintf(stderr, "メモリを確保できません (from allocSquareMatrix) \n");
        return NULL;
    }

    return matrix;
}

/*
solveLinearEquation(SquareMatrix *A, Vector *b)
  y = Ax を解く
          戻り値: 解ベクトル

          パラメータ
                入力
                    A: 係数行列
                    b: 定数ベクトル
*/

Vector *solveLinearEquation(const SquareMatrix *org_A, const Vector *org_y) {
    SquareMatrix *A;
    Vector *y;
    Vector *x;
    size_t i, j, k;

    // サイズ確認
    if (org_A->size != org_y->length) {
        fprintf(stderr, "Error: matrix size is not equal\n");
        return NULL;
    }

    // A と y へコピー
    A = (SquareMatrix *)copyMatrix((Matrix *)org_A);
    y = allocVector(org_y->length);
    for (i = 0; i < y->length; i++) {
        y->data[i] = org_y->data[i];
    }
    x = allocVector(y->length);

    for (i = 0; i < A->size; i++) {
        size_t max_row = 0;
        double max = 0.0;
        // 絶対最大値を持つ行を探す
        for (j = i; j < A->size; j++) {
            if (fabs(A->data[j][i]) > max) {
                max = fabs(A->data[j][i]);
                max_row = j;
            }
        }

        swapMatrixRow((Matrix *)A, i, max_row);
        swapVectorElement(y, i, max_row);

        // 前進消去
        for (j = i + 1; j < A->size; j++) {
            double tmp = A->data[j][i] / A->data[i][i];

            for (k = i; k < A->size; k++) {
                A->data[j][k] -= A->data[i][k] * tmp;
            }
            y->data[j] -= y->data[i] * tmp;
        }
    }

    // 後退代入
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
  Vector の i 番目と j 番目を入れ替える
          戻り値: 0: 正常終了, -1: 異常終了

          パラメータ
                入力
                    vector: 入れ替え対象ベクトル
                    i: 入れ替え対象要素番号
                    j: 入れ替え対象要素番号
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
  Matrix のサイズを変更する
          戻り値: Matrix 構造体へのポインタ

          パラメータ
                入力
                    matrix: サイズ変更対象行列
                    row: 変更後の行数
                    col: 変更後の列数
*/

Matrix *resizeMatrix(Matrix *matrix, size_t row, size_t col) {
    Matrix *res;
    size_t i;

    if ((res = allocMatrix(row, col)) == NULL) {
        fprintf(stderr, "メモリを確保できません (from resizeMatrix) \n");
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
  SquareMatrix のサイズを変更する
          戻り値: SquareMatrix 構造体へのポインタ

          パラメータ
                入力
                    matrix: サイズ変更対象行列
                    size: 変更後のサイズ
*/

SquareMatrix *resizeSquareMatrix(SquareMatrix *matrix, size_t size) {
    SquareMatrix *res;

    if ((res = (SquareMatrix *)resizeMatrix((Matrix *)matrix, size, size)) ==
        NULL) {
        fprintf(stderr, "メモリを確保できません (from resizeSquareMatrix) \n");
        return NULL;
    }

    return res;
}

/*
fillMatrix(Matrix *matrix, double value)
  Matrix を value で初期化する
          戻り値: なし

          パラメータ
                入力
                    matrix: 初期化対象行列
                    value: 初期化値
*/

void fillMatrix(Matrix *matrix, double value) {
    size_t i, j;

    for (i = 0; i < matrix->row; i++) {
        for (j = 0; j < matrix->col; j++) {
            matrix->data[i][j] = value;
        }
    }
}