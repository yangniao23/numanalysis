# ���l��� ���K�ۑ�5

## 1. �r���h��
- CPU: Intel(R) Xeon(R) CPU E5-2630 v4 @ 2.20GHz
- OS: Arch Linux (WSL2 5.15.123.1-microsoft-standard-WSL2)
- �R���p�C��: GNU Compiler Collection (GCC) 13.2.1 20230801
- C���C�u����: GNU C Library (GNU libc) stable release version 2.38.
- �r���h�c�[��: GNU Make 4.4.1

## 2. �r���h���@
1. zip�t�@�C����W�J����D
2. �W�J�����f�B���N�g���Ɉړ�����D
3. `make` �R�}���h�����s����D
4. ���s�t�@�C�� `k5.exe` �����������D

## 3. ���s���@
1. �r���h���@�̎菇1~4�����s����D
2. �]���p�f�[�^�t�@�C����ǉ�����D
    - ���s�t�@�C���Ɠ����f�B���N�g���ɔz�u����D
    - �t�@�C�����͌W���s��� `k5-input1.csv`�C�x�N�g���� `k5-input2.csv` �Ƃ���D
3. `k5.exe` �����s����D

## 4. �⑫
- �f�[�^�t�@�C���̃t�@�C������ `k5.c` ���� `MATRIX_FILENAME` �� `VECTOR_FILENAME` �Œ�`����Ă���D
- NABasic.c �̓��e���ꕔ�ύX���Ă���D��ȕύX�ӏ��������D
    - �x�N�g���E�s��̑傫���̌^�� `int` ���� `size_t` �ɕύX�����D
    - �֐� `copy_matrix` �� `swap_matrix_row` ��ǉ������D
- �����r���h�Ɏ��s�����ꍇ�́C`src\k5.c`, `src\lu_decomposition.c`, `lib\NABasic.c` ��3�̃t�@�C�����R���p�C�����Ď��s���Ă��������D

```
gcc -Wall -Werror -W -O2 -march=native -o k5.exe src\k5.c src\lu_decomposition.c lib\NAbasic.c -Ilib
```
