//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MATRIX_SIZE 8

typedef struct {
    int data[MATRIX_SIZE][MATRIX_SIZE];
} Matrix;

void matrix_init(Matrix *m) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            m->data[i][j] = i * MATRIX_SIZE + j;
        }
    }
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%4d", m->data[i][j]);
        }
        printf("\n");
    }
}

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return a + b;
}

int safe_multiply(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return a * b;
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            int sum = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                int product = safe_multiply(a->data[i][k], b->data[k][j]);
                if (product == 0 && (a->data[i][k] != 0 && b->data[k][j] != 0)) {
                    printf("Integer overflow detected in multiplication\n");
                    return;
                }
                sum = safe_add(sum, product);
                if (sum == 0 && product != 0) {
                    printf("Integer overflow detected in addition\n");
                    return;
                }
            }
            result->data[i][j] = sum;
        }
    }
}

void matrix_transpose(const Matrix *src, Matrix *dest) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
}

int matrix_trace(const Matrix *m) {
    int trace = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        trace = safe_add(trace, m->data[i][i]);
        if (trace == 0 && m->data[i][i] != 0) {
            printf("Integer overflow detected in trace calculation\n");
            return 0;
        }
    }
    return trace;
}

int main() {
    Matrix mat1, mat2, result;
    
    matrix_init(&mat1);
    matrix_init(&mat2);
    
    printf("Matrix A:\n");
    matrix_print(&mat1);
    printf("\nMatrix B:\n");
    matrix_print(&mat2);
    
    printf("\nMatrix multiplication A * B:\n");
    matrix_multiply(&mat1, &mat2, &result);
    matrix_print(&result);
    
    printf("\nTranspose of A:\n");
    matrix_transpose(&mat1, &result);
    matrix_print(&result);
    
    printf("\nTrace of A: %d\n", matrix_trace(&mat1));
    printf("Trace of B: %d\n", matrix_trace(&mat2));
    
    return 0;
}