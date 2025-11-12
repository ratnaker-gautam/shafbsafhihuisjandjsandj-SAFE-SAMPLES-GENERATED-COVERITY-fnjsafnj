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

Matrix multiply_recursive(Matrix a, Matrix b, int row, int col, int k) {
    Matrix result = create_matrix(a.rows, b.cols);
    if (row >= a.rows) return result;
    if (col >= b.cols) {
        return multiply_recursive(a, b, row + 1, 0, 0);
    }
    if (k >= a.cols) {
        result.data[row][col] = 0;
        return multiply_recursive(a, b, row, col + 1, 0);
    }
    Matrix partial = multiply_recursive(a, b, row, col, k + 1);
    partial.data[row][col] += a.data[row][k] * b.data[k][col];
    return partial;
}

Matrix matrix_multiply(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    return multiply_recursive(a, b, 0, 0, 0);
}

int main() {
    Matrix a, b;
    printf("Matrix A:\n");
    read_matrix(&a);
    printf("Matrix B:\n");
    read_matrix(&b);
    
    Matrix result = matrix_multiply(a, b);
    printf("Result of A * B:\n");
    print_matrix(result);
    
    return 0;
}