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

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            long sum = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    return 0;
                }
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    return 0;
                }
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

void matrix_transpose(const Matrix *src, Matrix *dest) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
}

int main() {
    Matrix a, b, result, transposed;
    
    matrix_init(&a);
    matrix_init(&b);
    
    printf("Matrix A:\n");
    matrix_print(&a);
    printf("\nMatrix B:\n");
    matrix_print(&b);
    
    if (!matrix_multiply(&a, &b, &result)) {
        printf("Matrix multiplication overflow detected\n");
        return 1;
    }
    
    printf("\nMatrix A * B:\n");
    matrix_print(&result);
    
    matrix_transpose(&result, &transposed);
    printf("\nTranspose of result:\n");
    matrix_print(&transposed);
    
    Matrix temp;
    if (!matrix_multiply(&result, &transposed, &temp)) {
        printf("Second multiplication overflow detected\n");
        return 1;
    }
    
    printf("\nResult * Transpose:\n");
    matrix_print(&temp);
    
    return 0;
}