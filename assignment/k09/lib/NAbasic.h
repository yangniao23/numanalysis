#pragma once
/*
        数値解析用基本関数群ヘッダファイル
        Dec. 3, 2023		ver 2.0
        Sept. 30, 2021		ver 1.2
        Oct. 03, 2018		ver 1.0
*/

// 必要ヘッダ
#include <stddef.h>
#include <stdio.h>

// 基本パラメータ定義
#define MAX_STR_LENGTH 200  // 1行の文字数の最大値
#define MAX_ROW_NUM 100     // csvファイルの最大行数

// 構造体定義
typedef struct {
    size_t row;  // 行数
    size_t col;  // 列数
    double **data;
} Matrix;

typedef struct {
    size_t size;
    size_t _size;  // Matrix 構造体との互換性のため
    double **data;
} SquareMatrix;

typedef struct {
    size_t length;  // ベクトルの長さ
    double *data;
} Vector;

// csvファイルを読み込み二次元配列に格納する
Matrix *csvRead(FILE *fin);
// vectorを確保する
Vector *allocVector(size_t length);
// matrixを確保する
Matrix *allocMatrix(size_t row, size_t col);
// matrixを開放する
void freeMatrix(Matrix *matrix);
// vectorを開放する
void freeVector(Vector *vector);
// matrixを列ベクトルへ変換する
Vector *matrix2colVector(const Matrix *matrix);
// matrixを表示する
void showMatrix(const Matrix *matrix);
// matrixを転置する
Matrix *tpMatrix(const Matrix *matrix);
// matrixのコピーを作成する
Matrix *copyMatrix(const Matrix *src);
// matrixのi行目とj行目を入れ替える (副作用あり)
int swapMatrixRow(Matrix *matrix, size_t i, size_t j);
// vector1とvector2の内積を計算する
double innerProduct(const Vector *vector1, const Vector *vector2);
// vectorのノルムを計算する
double vectorNorm(const Vector *vector);
// vectorを正規化する (副作用あり)
void normalizeVector(Vector *vector);
// matrix1とmatrix2の積を計算する (新しいmatrixを作成)
Matrix *matrixProduct(const Matrix *matrix1, const Matrix *matrix2);
// matrix1とmatrix2の積を計算する (resに結果を格納)
Matrix *matrixProduct2(Matrix *res, const Matrix *matrix1,
                       const Matrix *matrix2);
// matrix と vector の積を計算する (新しいvectorを作成)
Vector *matrixVectorProduct(const Matrix *matrix, const Vector *vector);
// matrix と vector の積を計算する (resに結果を格納)
Vector *matrixVectorProduct2(Vector *res, const Matrix *matrix,
                             const Vector *vector);
// vector を表示する
void showVector(const Vector *vector);
// SquareMatrixを確保する
SquareMatrix *allocSquareMatrix(size_t size);
// y = Ax を解く
Vector *solveLinearEquation(const SquareMatrix *org_A, const Vector *org_y);
// Vector の i 番目と j 番目を入れ替える (副作用あり)
int swapVectorElement(Vector *vector, size_t i, size_t j);