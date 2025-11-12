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
            int sum = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        return 0;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        return 0;
                    }
                }
                int product = a->data[i][k] * b->data[k][j];
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                sum += product;
            }
            result->data[i][j] = sum;
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
    
    Matrix identity;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            identity.data[i][j] = (i == j) ? 1 : 0;
        }
    }
    
    if (!matrix_multiply(&result, &identity, &transposed)) {
        printf("Identity multiplication overflow detected\n");
        return 1;
    }
    
    printf("\nResult * Identity (should equal result):\n");
    matrix_print(&transposed);
    
    int equal = 1;
    for (int i = 0; i < MATRIX_SIZE && equal; i++) {
        for (int j = 0; j < MATRIX_SIZE && equal; j++) {
            if (result.data[i][j] != transposed.data[i][j]) {
                equal = 0;
            }
        }
    }
    
    printf("\nVerification: %s\n", equal ? "PASS" : "FAIL");
    
    return 0;
}