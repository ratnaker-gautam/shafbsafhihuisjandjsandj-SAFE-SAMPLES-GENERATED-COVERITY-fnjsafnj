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

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            int sum = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        sum = INT_MAX;
                        break;
                    }
                    int product = a->data[i][k] * b->data[k][j];
                    if (sum > INT_MAX - product) {
                        sum = INT_MAX;
                        break;
                    }
                    sum += product;
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        sum = INT_MIN;
                        break;
                    }
                    int product = a->data[i][k] * b->data[k][j];
                    if (sum < INT_MIN - product) {
                        sum = INT_MIN;
                        break;
                    }
                    sum += product;
                } else {
                    int product = a->data[i][k] * b->data[k][j];
                    if ((product > 0 && sum > INT_MAX - product) ||
                        (product < 0 && sum < INT_MIN - product)) {
                        sum = (product > 0) ? INT_MAX : INT_MIN;
                        break;
                    }
                    sum += product;
                }
            }
            result->data[i][j] = sum;
        }
    }
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%8d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Matrix a, b, result;
    
    matrix_init(&a);
    matrix_init(&b);
    
    printf("Matrix A:\n");
    matrix_print(&a);
    printf("\nMatrix B:\n");
    matrix_print(&b);
    
    matrix_multiply(&a, &b, &result);
    
    printf("\nMatrix A * B:\n");
    matrix_print(&result);
    
    Matrix identity;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            identity.data[i][j] = (i == j) ? 1 : 0;
        }
    }
    
    matrix_multiply(&a, &identity, &result);
    
    printf("\nMatrix A * Identity:\n");
    matrix_print(&result);
    
    Matrix zero;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            zero.data[i][j] = 0;
        }
    }
    
    matrix_multiply(&a, &zero, &result);
    
    printf("\nMatrix A * Zero:\n");
    matrix_print(&result);
    
    return 0;
}