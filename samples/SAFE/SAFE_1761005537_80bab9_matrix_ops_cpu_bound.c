//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dim(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_matrix(Matrix *m) {
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        return 0;
    }
    if (!validate_matrix_dim(m->rows) || !validate_matrix_dim(m->cols)) {
        return 0;
    }
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%lf", &m->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    printf("%d %d\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    result->rows = a->rows;
    result->cols = b->cols;
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return 1;
}

int matrix_transpose(const Matrix *m, Matrix *result) {
    result->rows = m->cols;
    result->cols = m->rows;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
    return 1;
}

double matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        return 0.0;
    }
    double trace = 0.0;
    for (int i = 0; i < m->rows; i++) {
        trace += m->data[i][i];
    }
    return trace;
}

int main(void) {
    Matrix a, b, result;
    int operation;
    
    if (scanf("%d", &operation) != 1) {
        fprintf(stderr, "Invalid operation\n");
        return 1;
    }
    
    if (operation < 1 || operation > 3) {
        fprintf(stderr, "Invalid operation\n");
        return 1;
    }
    
    if (operation == 1 || operation == 2) {
        if (!read_matrix(&a)) {
            fprintf(stderr, "Invalid matrix A\n");
            return 1;
        }
    }
    
    if (operation == 1) {
        if (!read_matrix(&b)) {
            fprintf(stderr, "Invalid matrix B\n");
            return 1;
        }
        if (!matrix_multiply(&a, &b, &result)) {
            fprintf(stderr, "Matrix multiplication failed\n");
            return 1;
        }
        print_matrix(&result);
    } else if (operation == 2) {
        if (!matrix_transpose(&a, &result)) {
            fprintf(stderr, "Matrix transpose failed\n");
            return 1;
        }
        print_matrix(&result);
    } else if (operation == 3) {
        if (!read_matrix(&a)) {
            fprintf(stderr, "Invalid matrix A\n");
            return 1;
        }
        double trace = matrix_trace(&a);
        printf("%.2f\n", trace);
    }
    
    return 0;
}