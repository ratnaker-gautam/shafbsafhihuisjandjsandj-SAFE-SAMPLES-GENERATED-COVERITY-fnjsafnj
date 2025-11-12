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
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            m->data[i][j] = 0;
        }
    }
    m->rows = 0;
    m->cols = 0;
}

int read_matrix(Matrix *m) {
    int r, c;
    if (scanf("%d %d", &r, &c) != 2) return 0;
    if (r < MIN_DIM || r > MAX_ROWS || c < MIN_DIM || c > MAX_COLS) return 0;
    m->rows = r;
    m->cols = c;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    printf("%d %d\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (a->rows > MAX_ROWS || b->cols > MAX_COLS) return 0;
    init_matrix(result);
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

int main(void) {
    Matrix mat1, mat2, result;
    init_matrix(&mat1);
    init_matrix(&mat2);
    init_matrix(&result);
    
    printf("Enter first matrix (rows cols then elements):\n");
    if (!read_matrix(&mat1)) {
        printf("Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix (rows cols then elements):\n");
    if (!read_matrix(&mat2)) {
        printf("Invalid second matrix input\n");
        return 1;
    }
    
    if (!multiply_matrices(&mat1, &mat2, &result)) {
        printf("Matrix multiplication failed: dimension mismatch or integer overflow\n");
        return 1;
    }
    
    printf("Result matrix:\n");
    print_matrix(&result);
    
    return 0;
}