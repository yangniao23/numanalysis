# ���l��� ���K�ۑ�1�E2
## 1. �r���h��
- CPU: Intel(R) Xeon(R) CPU E5-2630 v4 @ 2.20GHz
- OS: Arch Linux (WSL2 5.15.123.1-microsoft-standard-WSL2)
- �R���p�C��: GNU Compiler Collection (GCC) 13.2.1 20230801
- C���C�u����: GNU C Library (GNU libc) stable release version 2.38.
- �r���h�c�[��: GNU Make 4.4.1

## 2. �r���h���@
1. zip�t�@�C����W�J����D
2. �W�J�����f�B���N�g���Ɉړ�����D
3. `lib` �f�B���N�g���Ɉړ����C���C�u���� `NAbasic.c`, `NAbasic.h` ��ǉ�����D
4. `k1` �f�B���N�g���Ɉړ����C`make` �R�}���h�����s����D
5. ���s�t�@�C�� `k1.exe` �����������D


## 3. ���s���@
1. �r���h���@�̎菇1~5�����s����D
2. �]���p�f�[�^�t�@�C����ǉ�����D
    - ���s�t�@�C���Ɠ����f�B���N�g���ɔz�u����D
    - �t�@�C������ `k1-input.csv` �Ƃ���D
3. `k1.exe` �����s����D

## 4. �⑫
- �G���f�B�A���̔���� ��W���}�N���� `__BYTE_ORDER__` �Ȃǂ��g�p���Ă��܂��DGCC �� Clang �ł͎����ŃG���f�B�A���̔��肪�\�ł����CMSVC �Ȃǈꕔ�̃R���p�C���ł́C�蓮�ŃG���f�B�A���̎w�肪�K�v�ƂȂ�ꍇ������܂��D�r�b�O�G���f�B�A���̃n�[�h�E�F�A�œ������ꍇ�� `k1.c`  �̃r���h���� `BIG_ENDIAN` �}�N����L���ɂ��Ă��������D�Ȃ��C���g���G���f�B�A���̃n�[�h�E�F�A�ɂ����Ă͓��ʂȍ�Ƃ͕s�v�ł��D