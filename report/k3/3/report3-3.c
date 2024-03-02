#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "NAbasic.h"

#define _PI 3.14159265358979323846
#define EPS 1e-7

typedef double (*func_t)(double);

void dump_romberg_table(size_t n, size_t p, Vector **T, size_t len) {
    size_t i, j;
    printf("division_num:%zu dimension:%zu\n", n, p);
    puts("Romberg Table");
    for (i = 0; i < len; i++) {
        for (j = 0; j < T[i]->length; j++) {
            printf("%.10f  ", T[i]->data[j]);
        }
        putchar('\n');
    }
}

double trapezoid(func_t f, double a, double b, size_t n) {
    double h = (b - a) / n;
    double sum = 0.0;
    size_t i;

    // sum = h*(f(a)/2 + f(a + h) + f(a + 2h) + ... + f(a + (n - 1)h) + f(b)/2)
    // where (a + nh = b)
    for (i = 1; i < n; i++) sum += f(a + i * h);
    sum += (f(a) + f(b)) / 2.0;

    return h * sum;
}

double simpson(func_t f, double a, double b, size_t n) {
    double h = (b - a) / 2.0 / n;
    double sum = 0.0;
    size_t i;

    // sum = h*(f(a) + 4*f(a + h) + 2*f(a + 2h) + ...  + f(b)/6)/3.0
    for (i = 1; i < 2 * n; i++) {
        size_t power = (i % 2 == 0) ? 2 : 4;
        sum += f(a + i * h) * power;
    }
    sum += f(a) + f(b);

    return h * sum / 3.0;
}

double romberg(func_t f, double a1, double a2, size_t k_max) {
    size_t i, k, m = 1;
    size_t n = 1, p = 1;  // n = 2^k, p = 4^m
    double result;
    Vector **T;
    bool is_converged = false;

    if ((T = (Vector **)malloc(sizeof(Vector *) * k_max)) == NULL) {
        fprintf(stderr, "Error: malloc() failed.\n");
        return NAN;
    }

    if ((T[0] = allocVector(1)) == NULL) {
        free(T);
        fprintf(stderr, "Error: allocVector() failed.\n");
        return NAN;
    }

    T[0]->data[0] = (a2 - a1) * (f(a1) + f(a2)) / 2.0;

    for (k = 1; ((k < k_max) && !(is_converged)); k++) {
        double h, sum = 0.0;

        if ((T[k] = allocVector(k + 1)) == NULL) {
            for (i = 0; i < k; i++) freeVector(T[i]);
            free(T);
            fprintf(stderr, "Error: allocVector() failed.\n");
            return NAN;
        }

        n *= 2;  // n = 2^k
        h = (a2 - a1) / n;
        // sum = f(a + h) + f(a + 3h) + ... + f(a+(2n-1)h)
        for (i = 1; i < n; i += 2) sum += f(a1 + i * h);
        // T[k][0] = T[k-1][0]/2 + h*sum (k >= 1)
        T[k]->data[0] = T[k - 1]->data[0] / 2 + h * sum;

        for (m = p = 1; m <= k; m++) {
            p *= 2;  // p = 2^m
            // T[k][m] = T[k][m-1] + (T[k][m-1] - T[k-1][m-1])/(p^2-1) (k >= 1)
            T[k]->data[m] =
                T[k]->data[m - 1] +
                (T[k]->data[m - 1] - T[k - 1]->data[m - 1]) / (p * p - 1);
            if (fabs(T[k]->data[m] - T[k - 1]->data[m - 1]) < EPS) {
                is_converged = true;
                T[k]->length = ++m;
                break;
            }
        }
    }

    // dump_romberg_table(n, p, T, k);
    result = T[k - 1]->data[m - 1];
    for (i = 0; i < k; i++) freeVector(T[i]);
    free(T);

    return result;
}

int main(void) {
    double res;
    double x_s = 0, x_e = _PI / 2.0;
    size_t n;
    func_t f = sin;
    double exact_value = 1.0;

    printf("Trapezoid\n");
    for (n = 1; n <= 1000; n++) {
        res = trapezoid(f, x_s, x_e, n);
        printf("%zu,%.10f,%.10f\n", n, res, fabs(res - exact_value));
    }

    printf("Simpson\n");
    for (n = 1; n <= 1000; n++) {
        res = simpson(f, x_s, x_e, n);
        printf("%zu,%.10f,%.10f\n", n, res, fabs(res - exact_value));
    }

    printf("Romberg\n");
    for (n = 1; n <= 10; n++) {
        res = romberg(f, x_s, x_e, n);
        printf("%zu,%.10f,%.10f\n", (long unsigned)pow(2, n - 1), res,
               fabs(res - exact_value));
    }
    return EXIT_SUCCESS;
}