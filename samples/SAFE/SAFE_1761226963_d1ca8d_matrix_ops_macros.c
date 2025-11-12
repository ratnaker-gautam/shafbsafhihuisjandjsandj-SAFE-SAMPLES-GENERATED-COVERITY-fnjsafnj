//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

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

void init_matrix(Matrix *m) {
    m->rows = 0;
    m->cols = 0;
    memset(m->data, 0, sizeof(m->data));
}

int read_matrix(Matrix *m) {
    int r, c;
    printf("Enter rows and columns (1-10): ");
    if (scanf("%d %d", &r, &c) != 2) return 0;
    if (r < MIN_DIM || r > MAX_ROWS || c < MIN_DIM || c > MAX_COLS) return 0;
    
    m->rows = r;
    m->cols = c;
    
    printf("Enter %d elements:\n", r * c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int product = safe_multiply(a->data[i][k], b->data[k][j]);
                if (product == 0 && (a->data[i][k] != 0 && b->data[k][j] != 0)) {
                    return 0;
                }
                sum = safe_add(sum, product);
                if (sum == 0 && product != 0) {
                    return 0;
                }
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *src, Matrix *dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    init_matrix(&mat1);
    init_matrix(&mat2);
    init_matrix(&result);
    
    printf("Matrix Operations\n");
    printf("1. Multiply matrices\n");
    printf("2. Transpose matrix\n");
    printf("Choice: ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Matrix 1:\n");
            if (!read_matrix(&mat1)) {
                printf("Invalid matrix 1\n");
                return 1;
            }
            printf("Matrix 2:\n");
            if (!read_matrix(&mat2)) {
                printf("Invalid matrix 2\n");
                return 1;
            }
            
            if (multiply_matrices(&mat1, &mat2, &result)) {
                printf("Result:\n");
                print_matrix(&result);
            } else {
                printf("Multiplication failed\n");
            }
            break;
            
        case 2:
            printf("Matrix:\n");
            if (!read_matrix(&mat1)) {
                printf("Invalid matrix\n");
                return 1;
            }
            
            if (transpose_matrix(&mat1, &result)) {
                printf("Transpose:\n");
                print_matrix(&result);
            } else {
                printf("Transpose failed\n");
            }
            break;
            
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}