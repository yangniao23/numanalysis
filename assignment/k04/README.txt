# 数値解析 演習課題4

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
4. 実行ファイル `k4.exe` が生成される．

## 3. 実行方法
1. ビルド方法の手順1~4を実行する．
2. 評価用データファイルを追加する．
    - 実行ファイルと同じディレクトリに配置する．
    - ファイル名は係数行列を `k4-input1.csv`，ベクトルを `k4-input2.csv` とする．
3. `k4.exe` を実行する．

## 4. 補足
Makefile が上手く動作しない場合は，`gcc -o k4.exe src\k4.c lib\NAbasic.c -Ilib` みたいな感じで手動コンパイルをお願いします．
