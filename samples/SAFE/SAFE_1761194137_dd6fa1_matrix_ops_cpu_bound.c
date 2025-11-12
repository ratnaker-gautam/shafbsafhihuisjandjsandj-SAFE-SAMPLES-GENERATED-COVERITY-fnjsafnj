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
                if ((a->data[i][k] > 0 && b->data[k][j] > INT_MAX / a->data[i][k]) ||
                    (a->data[i][k] < 0 && b->data[k][j] < INT_MIN / a->data[i][k])) {
                    return 0;
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
    
    printf("\nMatrix multiplication result:\n");
    if (matrix_multiply(&a, &b, &result)) {
        matrix_print(&result);
    } else {
        printf("Error: Integer overflow during multiplication\n");
        return 1;
    }
    
    printf("\nTranspose of result matrix:\n");
    matrix_transpose(&result, &transposed);
    matrix_print(&transposed);
    
    printf("\nVerification: Transpose of transpose equals original:\n");
    Matrix verify;
    matrix_transpose(&transposed, &verify);
    
    int equal = 1;
    for (int i = 0; i < MATRIX_SIZE && equal; i++) {
        for (int j = 0; j < MATRIX_SIZE && equal; j++) {
            if (verify.data[i][j] != result.data[i][j]) {
                equal = 0;
            }
        }
    }
    
    if (equal) {
        printf("Verification successful\n");
    } else {
        printf("Verification failed\n");
        return 1;
    }
    
    return 0;
}