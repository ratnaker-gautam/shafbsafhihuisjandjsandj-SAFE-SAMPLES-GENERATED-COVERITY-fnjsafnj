//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

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
    printf("Enter matrix dimensions (rows cols): ");
    int rows, cols;
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input\n");
        exit(1);
    }
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix size\n");
        exit(1);
    }
    m->rows = rows;
    m->cols = cols;
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
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
            printf("%d ", m.data[i][j]);
        }
        printf("\n");
    }
}

int recursive_trace(Matrix m, int row, int col, int sum) {
    if (row >= m.rows || col >= m.cols) return sum;
    if (row == col) {
        return recursive_trace(m, row + 1, col + 1, sum + m.data[row][col]);
    }
    return recursive_trace(m, row + 1, col + 1, sum);
}

int matrix_trace(Matrix m) {
    if (m.rows != m.cols) return 0;
    return recursive_trace(m, 0, 0, 0);
}

Matrix recursive_transpose(Matrix m, Matrix result, int row, int col) {
    if (row >= m.rows) return result;
    if (col >= m.cols) {
        return recursive_transpose(m, result, row + 1, 0);
    }
    result.data[col][row] = m.data[row][col];
    return recursive_transpose(m, result, row, col + 1);
}

Matrix matrix_transpose(Matrix m) {
    Matrix result = create_matrix(m.cols, m.rows);
    return recursive_transpose(m, result, 0, 0);
}

int main() {
    Matrix A;
    read_matrix(&A);
    
    printf("Original matrix:\n");
    print_matrix(A);
    
    int trace = matrix_trace(A);
    if (trace != 0) {
        printf("Matrix trace: %d\n", trace);
    } else {
        printf("Matrix is not square, cannot compute trace\n");
    }
    
    Matrix B = matrix_transpose(A);
    printf("Transposed matrix:\n");
    print_matrix(B);
    
    return 0;
}