# 数値解析 演習課題12

## 1. ビルド環境
- CPU: Intel(R) Xeon(R) CPU E5-2630 v4 @ 2.20GHz
- OS: Arch Linux (WSL2 5.15.123.1-microsoft-standard-WSL2)
- コンパイラ: GNU Compiler Collection (GCC) 13.2.1 20230801
- Cライブラリ: GNU C Library (GNU libc) stable release version 2.38.
- ビルドツール: GNU Make 4.4.1

## 2. ビルド方法
1. zipファイルを展開する．
2. `BCC102 開発環境` を起動し，展開したディレクトリに移動する．
3. `make` コマンドを実行する．
4. 実行ファイル `k12.exe` が生成される．

## 3. 実行方法
1. ビルド方法の手順1~4を実行する．
2. 評価用データファイルを追加する．
    - 実行ファイルと同じディレクトリに配置する．
    - ファイル名は `k12-input.csv`とする．
3. `k12.exe` を実行する．

## 4. 補足
- データファイルのファイル名は `src\k12.c` 中の `PARAM_FILENAME` で定義されている．
- 被計算関数は `src\k12.c` 中の `double df(double x, double y)` で定義されている．
- NABasic.c の内容を大きく変更している．主な変更箇所を示す．詳細は`lib\changelog.txt`に記載した．
    - ベクトル・行列を構造体 Vector と Matrix を用いて宣言するようにした．
    - エラーハンドリングの自由度を上げるために，メモリ確保失敗時には確保済のメモリを解放してから `NULL` を返すように変更した．
- ビルドに失敗した場合は以下のように手動でコンパイルしてください．

```
gcc -O2 -march=native -o k12.exe src\k12.c lib\NAbasic.c -Ilib -Llib -lm
```
