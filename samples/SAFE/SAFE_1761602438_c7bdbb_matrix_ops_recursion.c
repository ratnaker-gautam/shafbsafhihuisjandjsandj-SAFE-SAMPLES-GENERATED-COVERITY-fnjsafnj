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

Matrix create_matrix(int rows, int cols) {
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m.data[i][j] = 0;
        }
    }
    return m;
}

int validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    return 1;
}

void read_matrix(Matrix *m) {
    printf("Enter matrix elements (%dx%d):\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &m->data[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

void print_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%6d", m.data[i][j]);
        }
        printf("\n");
    }
}

Matrix recursive_multiply(Matrix a, Matrix b, int row, int col, int k) {
    Matrix result = create_matrix(a.rows, b.cols);
    
    if (row >= a.rows) return result;
    if (col >= b.cols) {
        return recursive_multiply(a, b, row + 1, 0, 0);
    }
    if (k >= a.cols) {
        result.data[row][col] = 0;
        return recursive_multiply(a, b, row, col + 1, 0);
    }
    
    Matrix partial = recursive_multiply(a, b, row, col, k + 1);
    partial.data[row][col] += a.data[row][k] * b.data[k][col];
    return partial;
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    return recursive_multiply(a, b, 0, 0, 0);
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions of first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (!validate_matrix_size(rows1, cols1)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions of second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (!validate_matrix_size(rows2, cols2)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    Matrix mat1 = create_matrix(rows1, cols1);
    Matrix mat2 = create_matrix(rows2, cols2);
    
    read_matrix(&mat1);
    read_matrix(&mat2);
    
    printf("\nFirst matrix:\n");
    print_matrix(mat1);
    printf("\nSecond matrix:\n");
    print_matrix(mat2);
    
    Matrix result = multiply_matrices(mat1, mat2);
    
    printf("\nResult matrix:\n");
    print_matrix(result);
    
    return 0;
}