#include "NAbasic.h"

#define INTERVAL_FILENAME "k3-input.csv"
#define BISECTION_METHOD 0
#define NEWTON_METHOD 1
#define LOOP_COUNT_MAX 10

#define CLIST_NOW(position) (position % 2)
#define CLIST_PREV(position) ((position + 1) % 2)

typedef double (*FUNC_D2D)(double);

double target_function(double x) {
    // x^4 - 3x^2 + 5x - 9
    return x * x * x * x - 3 * x * x + 5 * x - 9;
}

double target_function_differentiated(double x) {
    // 4x^3 - 6x + 5
    return 4 * x * x * x - 6 * x + 5;
}

void print_current_result(int mode, int loop_count, double current_result) {
    if (mode == BISECTION_METHOD) {
        printf(
            "bisection method: remain loop_count: %d, current_result: %.9f\n",
            LOOP_COUNT_MAX - loop_count, current_result);
    } else if (mode == NEWTON_METHOD) {
        printf("newton method: loop_count: %d, current_result: %.9f\n",
               loop_count, current_result);
    } else {
        fprintf(stderr, "Error: Invalid mode.\n");
        exit(EXIT_FAILURE);
    }
}

void bisection_method(FUNC_D2D f, double x_min, double x_max) {
    int loop_count;

    // The loop starts at 1 and runs LOOP_COUNT_MAX times.
    for (loop_count = 1; (loop_count <= LOOP_COUNT_MAX); loop_count++) {
        double x_mid = (x_min + x_max) / 2;
        print_current_result(BISECTION_METHOD, loop_count, x_mid);

        if (f(x_mid) * f(x_min) < 0) {
            x_max = x_mid;
        } else if (f(x_mid) * f(x_max) < 0) {
            x_min = x_mid;
        } else {
            return;
        }
    }
}

void newton_method(FUNC_D2D f, FUNC_D2D df, double x_min, double x_max) {
    double x[2], y[2], difference;
    int loop_count = 1;  // loop_count starts from 1 because newton method needs
                         // to calculate initial value at first.
    x[0] = (x_min + x_max) / 2;
    y[0] = f(x[0]);
    difference = df(x[0]);

    for (loop_count = 1; loop_count <= LOOP_COUNT_MAX; loop_count++) {
        x[CLIST_NOW(loop_count)] =
            x[CLIST_PREV(loop_count)] - y[CLIST_PREV(loop_count)] / difference;
        y[CLIST_NOW(loop_count)] = f(x[CLIST_NOW(loop_count)]);
        difference = df(x[CLIST_NOW(loop_count)]);

        print_current_result(NEWTON_METHOD, loop_count,
                             x[CLIST_NOW(loop_count)]);
    }
}

int main(void) {
    FILE *matrix_fin;
    double **matrix, x_min, x_max;
    int row, column;

    if ((matrix_fin = fopen(INTERVAL_FILENAME, "r")) == NULL) {
        fprintf(stderr, "file %s is not found.\n", INTERVAL_FILENAME);
        return EXIT_FAILURE;
    }

    // Error handlings are processed in csvRead(), so omitted here.
    matrix = csvRead(&row, &column, matrix_fin);
    fclose(matrix_fin);

    // check if the matrix has a only [a1, a2]
    if (row != 1 || column != 2) {
        printf("Error: Matrix sizes do not match.\n");
        freeMatrix(matrix);
        return EXIT_FAILURE;
    }

    x_min = matrix[0][0];
    x_max = matrix[0][1];
    freeMatrix(matrix);

    bisection_method(target_function, x_min, x_max);
    newton_method(target_function, target_function_differentiated, x_min,
                  x_max);
}