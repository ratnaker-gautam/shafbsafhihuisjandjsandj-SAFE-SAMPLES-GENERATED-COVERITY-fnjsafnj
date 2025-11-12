//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MAX_ELEM 1000
#define MIN_ELEM -1000

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
    printf("Enter rows and columns (1-10): ");
    if (scanf("%d %d", &r, &c) != 2) return 0;
    if (r < MIN_DIM || r > MAX_ROWS || c < MIN_DIM || c > MAX_COLS) return 0;
    
    m->rows = r;
    m->cols = c;
    
    printf("Enter %d elements:\n", r * c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            int val;
            if (scanf("%d", &val) != 1) return 0;
            if (val < MIN_ELEM || val > MAX_ELEM) return 0;
            m->data[i][j] = val;
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
    
    init_matrix(result);
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int prod = safe_multiply(a->data[i][k], b->data[k][j]);
                if (prod == 0 && (a->data[i][k] != 0 && b->data[k][j] != 0)) {
                    return 0;
                }
                sum = safe_add(sum, prod);
                if (sum == 0 && prod != 0) {
                    return 0;
                }
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *src, Matrix *dest) {
    if (src->rows > MAX_COLS || src->cols > MAX_ROWS) return 0;
    
    init_matrix(dest);
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations\n");
    printf("1. Multiply matrices\n");
    printf("2. Transpose matrix\n");
    printf("Enter choice: ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Matrix A:\n");
            if (!read_matrix(&a)) {
                printf("Invalid matrix A\n");
                return 1;
            }
            printf("Matrix B:\n");
            if (!read_matrix(&b)) {
                printf("Invalid matrix B\n");
                return 1;
            }
            if (multiply_matrices(&a, &b, &result)) {
                printf("Result:\n");
                print_matrix(&result);
            } else {
                printf("Cannot multiply: dimension mismatch or overflow\n");
            }
            break;
            
        case 2:
            printf("Matrix to transpose:\n