#pragma once

#include <stddef.h>

typedef struct {
    double **l_matrix;
    double **u_matrix;
    double **p_matrix;
} lu_decomposition_t;

extern double *solve_simultaneous_by_lu_decomposition(const double **org_matrix,
                                                      const double *org_vector,
                                                      size_t matrix_size,
                                                      lu_decomposition_t *lu);