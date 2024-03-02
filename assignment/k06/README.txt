# 数値解析 演習課題6

## 1. ビルド環境
- CPU: Intel(R) Xeon(R) CPU E5-2630 v4 @ 2.20GHz
- OS: Arch Linux (WSL2 5.15.123.1-microsoft-standard-WSL2)
- コンパイラ: GNU Compiler Collection (GCC) 13.2.1 20230801
- Cライブラリ: GNU C Library (GNU libc) stable release version 2.38.
- ビルドツール: GNU Make 4.4.1

## 2. ビルド方法
1. zipファイルを展開する．
2. 展開したディレクトリに移動する．
3. `make` コマンドを実行する．
4. 実行ファイル `k6.exe` が生成される．

## 3. 実行方法
1. ビルド方法の手順1~4を実行する．
2. 評価用データファイルを追加する．
    - 実行ファイルと同じディレクトリに配置する．
    - ファイル名は係数行列を `k6-input1.csv`，ベクトルを `k6-input2.csv` とする．
3. `k5.exe` を実行する．

## 4. 補足
- データファイルのファイル名は `k6.c` 中の `MATRIX_FILENAME` と `VECTOR_FILENAME` で定義されている．
- それぞれのループ実行回数は `k6.c` 中の `JACOBI_LOOP_MAX` 及び `GAUSS_SEIDAL_LOOP_MAX` で定義されている．
- NABasic.c の内容を一部変更している．主な変更箇所を示す．
    - ベクトル・行列の大きさの型を `int` から `size_t` に変更した．
- ビルドに失敗した場合は以下のように手動でコンパイルしてください．

```
gcc -Wall -Werror -W -O2 -march=native -o k6.exe src\k6.c lib\NAbasic.c -Ilib
```
