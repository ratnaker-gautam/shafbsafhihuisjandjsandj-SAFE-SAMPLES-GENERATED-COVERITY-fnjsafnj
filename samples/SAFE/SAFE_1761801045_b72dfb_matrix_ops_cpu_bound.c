//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MATRIX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MATRIX_SIZE][MATRIX_SIZE];
} Matrix;

void matrix_init(Matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MATRIX_SIZE || cols > MATRIX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = 0;
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    if (result->rows != a->rows || result->cols != b->cols) {
        fprintf(stderr, "Result matrix has wrong dimensions\n");
        exit(1);
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((a->data[i][k] > 0 && b->data[k][j] > INT_MAX / a->data[i][k]) ||
                    (a->data[i][k] < 0 && b->data[k][j] < INT_MIN / a->data[i][k])) {
                    fprintf(stderr, "Integer overflow in multiplication\n");
                    exit(1);
                }
                int product = a->data[i][k] * b->data[k][j];
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    fprintf(stderr, "Integer overflow in summation\n");
                    exit(1);
                }
                sum += product;
            }
            result->data[i][j] = sum;
        }
    }
}

void matrix_transpose(const Matrix *src, Matrix *dst) {
    if (src->rows != dst->cols || src->cols != dst->rows) {
        fprintf(stderr, "Transpose dimensions mismatch\n");
        exit(1);
    }
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Matrix a, b, result, transposed;
    
    matrix_init(&a, 3, 3);
    matrix_init(&b, 3, 3);
    matrix_init(&result, 3, 3);
    matrix_init(&transposed, 3, 3);
    
    int counter = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a.data[i][j] = counter++;
            b.data[i][j] = counter % 5 + 1;
        }
    }
    
    printf("Matrix A:\n");
    matrix_print(&a);
    printf("\nMatrix B:\n");
    matrix_print(&b);
    
    matrix_multiply(&a, &b, &result);
    printf("\nMatrix A * B:\n");
    matrix_print(&result);
    
    matrix_transpose(&result, &transposed);
    printf("\nTranspose of result:\n");
    matrix_print(&transposed);
    
    Matrix c, d, result2;
    matrix_init(&c, 2, 4);
    matrix_init(&d, 4, 2);
    matrix_init(&result2, 2, 2);
    
    int val = 1;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            c.data[i][j] = val++;
        }
    }
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            d.data[i][j] = (i + j) % 3 + 1;
        }
    }
    
    printf("\nMatrix C:\n");
    matrix_print(&c);
    printf("\nMatrix D:\n");
    matrix_print(&d);
    
    matrix_multiply(&c, &d, &result2);
    printf("\nMatrix C * D:\n");
    matrix_print(&result2);
    
    return 0;
}