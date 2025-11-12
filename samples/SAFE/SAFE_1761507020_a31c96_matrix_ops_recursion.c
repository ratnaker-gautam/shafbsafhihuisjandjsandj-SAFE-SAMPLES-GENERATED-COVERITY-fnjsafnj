//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void print_matrix_recursive(Matrix *m, int row, int col) {
    if (row >= m->rows) return;
    
    printf("%6d", m->data[row][col]);
    
    if (col + 1 < m->cols) {
        print_matrix_recursive(m, row, col + 1);
    } else {
        printf("\n");
        print_matrix_recursive(m, row + 1, 0);
    }
}

void read_matrix_recursive(Matrix *m, int row, int col) {
    if (row >= m->rows) return;
    
    printf("Enter element [%d][%d]: ", row, col);
    int value;
    if (scanf("%d", &value) != 1) {
        printf("Invalid input\n");
        exit(1);
    }
    
    m->data[row][col] = value;
    
    if (col + 1 < m->cols) {
        read_matrix_recursive(m, row, col + 1);
    } else {
        read_matrix_recursive(m, row + 1, 0);
    }
}

void transpose_recursive(Matrix *src, Matrix *dst, int row, int col) {
    if (row >= src->rows) return;
    
    dst->data[col][row] = src->data[row][col];
    
    if (col + 1 < src->cols) {
        transpose_recursive(src, dst, row, col + 1);
    } else {
        transpose_recursive(src, dst, row + 1, 0);
    }
}

int determinant_recursive(Matrix *m, int size) {
    if (size == 1) return m->data[0][0];
    if (size == 2) {
        return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
    }
    
    int det = 0;
    int sign = 1;
    Matrix submatrix;
    submatrix.rows = size - 1;
    submatrix.cols = size - 1;
    
    for (int col = 0; col < size; col++) {
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                submatrix.data[sub_i][sub_j] = m->data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        int sub_det = determinant_recursive(&submatrix, size - 1);
        if (sub_det > 0 && sign > 0 && det > INT_MAX - sub_det) exit(1);
        if (sub_det < 0 && sign > 0 && det < INT_MIN - sub_det) exit(1);
        if (sub_det > 0 && sign < 0 && det < INT_MIN + sub_det) exit(1);
        if (sub_det < 0 && sign < 0 && det > INT_MAX + sub_det) exit(1);
        
        det += sign * m->data[0][col] * sub_det;
        sign = -sign;
    }
    
    return det;
}

int main() {
    Matrix mat, transposed;
    
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_SIZE, MAX_SIZE);
    if (scanf("%d %d", &mat.rows, &mat.cols) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    if (!validate_matrix_size(mat.rows, mat.cols)) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter matrix elements:\n");
    read_matrix_recursive(&mat, 0, 0);
    
    printf("\nOriginal matrix:\n");
    print_matrix_recursive(&mat, 0, 0);
    
    if (mat.rows == mat.cols) {
        int det = determinant_recursive(&mat, mat.rows);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nMatrix is not square, skipping determinant\n");
    }
    
    transposed.rows = mat.cols;
    transposed.cols = mat.rows;
    transpose_recursive(&mat, &transposed, 0, 0);
    
    printf("\nTransposed matrix:\n");
    print_matrix_recursive(&transposed, 0, 0);
    
    return 0;
}