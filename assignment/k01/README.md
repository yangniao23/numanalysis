# 数値解析 演習課題1・2
## 1. ビルド環境
- CPU: Intel(R) Xeon(R) CPU E5-2630 v4 @ 2.20GHz
- OS: Arch Linux (WSL2 5.15.123.1-microsoft-standard-WSL2)
- コンパイラ: GNU Compiler Collection (GCC) 13.2.1 20230801
- Cライブラリ: GNU C Library (GNU libc) stable release version 2.38.
- ビルドツール: GNU Make 4.4.1

## 2. ビルド方法
1. zipファイルを展開する．
2. 展開したディレクトリに移動する．
3. `lib` ディレクトリに移動し，ライブラリ `NAbasic.c`, `NAbasic.h` を追加する．
4. `k1` ディレクトリに移動し，`make` コマンドを実行する．
5. 実行ファイル `k1.exe` が生成される．


## 3. 実行方法
1. ビルド方法の手順1~5を実行する．
2. 評価用データファイルを追加する．
    - 実行ファイルと同じディレクトリに配置する．
    - ファイル名は `k1-input.csv` とする．
3. `k1.exe` を実行する．

## 4. 補足
- エンディアンの判定に 非標準マクロの `__BYTE_ORDER__` などを使用しています．GCC や Clang では自動でエンディアンの判定が可能ですが，MSVC など一部のコンパイラでは，手動でエンディアンの指定が必要となる場合があります．ビッグエンディアンのハードウェアで動かす場合は `k1.c`  のビルド時に `BIG_ENDIAN` マクロを有効にしてください．なお，リトルエンディアンのハードウェアにおいては特別な作業は不要です．