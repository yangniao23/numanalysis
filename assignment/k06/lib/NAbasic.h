/*
        数値解析用基本関数群ヘッダファイル
        Dec. 3, 2023		ver 1.3
        Sept. 30, 2021		ver 1.2
        Oct. 03, 2018		ver 1.0
*/

// 必要ヘッダ
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 基本パラメータ定義
#define MAX_STR_LENGTH 200  // 1行の文字数の最大値
#define MAX_ROW_NUM 100     // csvファイルの最大行数

// csvファイルを読み込み二次元配列に格納する
double **csvRead(size_t *row, size_t *col, FILE *fin);
// vectorを確保する
double *allocVector(size_t length);
// matrixを確保する
double **allocMatrix(size_t row, size_t col);
// matrixを開放する
int freeMatrix(double **matrix);
// vectorを開放する
int freeVector(double *vector);
// matrixを列ベクトルへ変換する
double *matrix2colVector(const double **matrix, size_t row, size_t col);
// matrixを表示する
int showMatrix(const double **matrix, size_t row, size_t col);
// matrixを転置する
double **tpMatrix(const double **matrix, size_t row, size_t col);
// matrixのコピーを作成する
double **copy_matrix(const double **src, double **dst, size_t row, size_t col);
// matrixのi行目とj行目を入れ替える
int swap_matrix_row(double **matrix, size_t row, size_t i, size_t j);