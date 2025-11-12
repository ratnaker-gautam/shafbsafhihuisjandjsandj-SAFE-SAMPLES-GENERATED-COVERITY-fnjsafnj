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

int determinant_recursive(Matrix *m, int size, int temp[MAX_SIZE][MAX_SIZE]) {
    if (size == 1) return m->data[0][0];
    
    int det = 0;
    int sign = 1;
    
    for (int f = 0; f < size; f++) {
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == f) continue;
                temp[sub_i][sub_j] = m->data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        Matrix submatrix = {size - 1, size - 1};
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - 1; j++) {
                submatrix.data[i][j] = temp[i][j];
            }
        }
        
        int sub_det = determinant_recursive(&submatrix, size - 1, temp);
        if (sign == 1) {
            if (det > INT_MAX - m->data[0][f] * sub_det) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            det += m->data[0][f] * sub_det * sign;
        } else {
            if (det < INT_MIN + m->data[0][f] * sub_det) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            det += m->data[0][f] * sub_det * sign;
        }
        sign = -sign;
    }
    
    return det;
}

int main() {
    Matrix mat, transposed;
    int rows, cols;
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    mat.rows = rows;
    mat.cols = cols;
    
    printf("Enter matrix elements:\n");
    read_matrix_recursive(&mat, 0, 0);
    
    printf("\nOriginal matrix:\n");
    print_matrix_recursive(&mat, 0, 0);
    
    if (rows == cols) {
        int temp[MAX_SIZE][MAX_SIZE];
        int det = determinant_recursive(&mat, rows, temp);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nMatrix is not square, skipping determinant\n");
    }
    
    transposed.rows = cols;
    transposed.cols = rows;
    transpose_recursive(&mat, &transposed, 0, 0);
    
    printf("\nTransposed matrix:\n");
    print_matrix_recursive(&transposed, 0, 0);
    
    return 0;
}