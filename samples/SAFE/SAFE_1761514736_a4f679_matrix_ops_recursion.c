//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    return (rows > 0 && rows <= MAX_DIM && cols > 0 && cols <= MAX_DIM);
}

void read_matrix(Matrix *m) {
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_DIM, MAX_DIM);
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        printf("Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_dimensions(m->rows, m->cols)) {
        printf("Invalid dimensions\n");
        exit(1);
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int recursive_determinant(const Matrix *m, int size, int row_mask, int col_mask) {
    if (size == 1) {
        for (int i = 0; i < m->rows; i++) {
            if (!(row_mask & (1 << i))) {
                for (int j = 0; j < m->cols; j++) {
                    if (!(col_mask & (1 << j))) {
                        return m->data[i][j];
                    }
                }
            }
        }
    }
    
    int det = 0;
    int sign = 1;
    int first_row = -1;
    
    for (int i = 0; i < m->rows; i++) {
        if (!(row_mask & (1 << i))) {
            first_row = i;
            break;
        }
    }
    
    if (first_row == -1) return 0;
    
    for (int j = 0; j < m->cols; j++) {
        if (!(col_mask & (1 << j))) {
            int minor = recursive_determinant(m, size - 1, row_mask | (1 << first_row), col_mask | (1 << j));
            if (minor != INT_MIN && det != INT_MIN) {
                long long product = (long long)sign * m->data[first_row][j] * minor;
                if (product > INT_MAX || product < INT_MIN) {
                    det = INT_MIN;
                } else {
                    det += (int)product;
                }
            } else {
                det = INT_MIN;
            }
            sign = -sign;
        }
    }
    
    return det;
}

int compute_determinant(const Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for determinant\n");
        return INT_MIN;
    }
    
    if (m->rows == 0) return 1;
    
    return recursive_determinant(m, m->rows, 0, 0);
}

void recursive_transpose(const Matrix *src, Matrix *dst, int row, int col) {
    if (row >= src->rows) return;
    
    if (col >= src->cols) {
        recursive_transpose(src, dst, row + 1, 0);
        return;
    }
    
    dst->data[col][row] = src->data[row][col];
    recursive_transpose(src, dst, row, col + 1);
}

void transpose_matrix(const Matrix *src, Matrix *dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    recursive_transpose(src, dst, 0, 0);
}

int main() {
    Matrix mat, transposed;
    
    printf("Matrix Operations using Recursion\n");
    printf("=================================\n");
    
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    int det = compute_determinant(&mat);
    if (det != INT_MIN) {
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nDeterminant calculation failed (overflow or error)\n");
    }
    
    if (mat.rows > 0 && mat.cols > 0) {
        transpose_matrix(&mat, &transposed);
        printf("\nTransposed matrix:\n");
        print_matrix(&transposed);
    }
    
    return 0;
}