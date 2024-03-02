# ���l��� ���K�ۑ�12

## 1. �r���h��
- CPU: Intel(R) Xeon(R) CPU E5-2630 v4 @ 2.20GHz
- OS: Arch Linux (WSL2 5.15.123.1-microsoft-standard-WSL2)
- �R���p�C��: GNU Compiler Collection (GCC) 13.2.1 20230801
- C���C�u����: GNU C Library (GNU libc) stable release version 2.38.
- �r���h�c�[��: GNU Make 4.4.1

## 2. �r���h���@
1. zip�t�@�C����W�J����D
2. `BCC102 �J����` ���N�����C�W�J�����f�B���N�g���Ɉړ�����D
3. `make` �R�}���h�����s����D
4. ���s�t�@�C�� `k12.exe` �����������D

## 3. ���s���@
1. �r���h���@�̎菇1~4�����s����D
2. �]���p�f�[�^�t�@�C����ǉ�����D
    - ���s�t�@�C���Ɠ����f�B���N�g���ɔz�u����D
    - �t�@�C������ `k12-input.csv`�Ƃ���D
3. `k12.exe` �����s����D

## 4. �⑫
- �f�[�^�t�@�C���̃t�@�C������ `src\k12.c` ���� `PARAM_FILENAME` �Œ�`����Ă���D
- ��v�Z�֐��� `src\k12.c` ���� `double df(double x, double y)` �Œ�`����Ă���D
- NABasic.c �̓��e��傫���ύX���Ă���D��ȕύX�ӏ��������D�ڍׂ�`lib\changelog.txt`�ɋL�ڂ����D
    - �x�N�g���E�s����\���� Vector �� Matrix ��p���Đ錾����悤�ɂ����D
    - �G���[�n���h�����O�̎��R�x���グ�邽�߂ɁC�������m�ێ��s���ɂ͊m�ۍς̃�������������Ă��� `NULL` ��Ԃ��悤�ɕύX�����D
- �r���h�Ɏ��s�����ꍇ�͈ȉ��̂悤�Ɏ蓮�ŃR���p�C�����Ă��������D

```
gcc -O2 -march=native -o k12.exe src\k12.c lib\NAbasic.c -Ilib -Llib -lm
```
