#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define FILENAME "k1-input.csv"

#if __BYTE_ORDER__ && __ORDER_LITTLE_ENDIAN__ && __ORDER_BIG_ENDIAN__
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #undef BIG_ENDIAN
    #else
        #define BIG_ENDIAN 1
    #endif
#endif

void bitdump_double(double d) {
    size_t i;
    int j;
    char c[sizeof(double)];

    memcpy(c, &d, sizeof(double));
    for (i = 0; i < sizeof(double); i++) {
        for (j = CHAR_BIT - 1; j >= 0; j--) {
#ifdef BIG_ENDIAN
            if (c[i] & (1 << j))
#else  // for little endian; another endian system does not supported
            if (c[sizeof(double) - (i + 1)] & (1 << j))
#endif
                putchar('1');
            else
                putchar('0');
        }
        putchar(' ');
    }
    putchar('\n');
}

int main(void) {
    FILE *fin;
    double **matrix;
    int row, col;
    int i, j;

    if ((fin = fopen(FILENAME, "r")) == NULL) {
        fprintf(stderr, "file %s is not found\n", FILENAME);
        return EXIT_FAILURE;
    }

    // Error handlings are processed in csvRead(), so omitted here.
    matrix = csvRead(&row, &col, fin);
    fclose(fin);

    // IEEE 754 double precision format
    printf("sEEEEEEE EEEEdddd dddddddd... \n");

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            bitdump_double(matrix[i][j]);
        }
    }
    freeMatrix(matrix);

    return EXIT_SUCCESS;
}