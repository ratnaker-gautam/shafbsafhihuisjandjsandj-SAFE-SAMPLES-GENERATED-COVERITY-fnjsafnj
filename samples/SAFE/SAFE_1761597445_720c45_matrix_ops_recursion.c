//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

void print_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%d ", m.data[i][j]);
        }
        printf("\n");
    }
}

void read_matrix_input(Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        printf("Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_size(m->rows, m->cols)) {
        printf("Invalid matrix size\n");
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

Matrix recursive_transpose(Matrix m, int row, int col) {
    Matrix result = create_matrix(m.cols, m.rows);
    
    if (row >= m.rows) {
        return result;
    }
    
    if (col >= m.cols) {
        return recursive_transpose(m, row + 1, 0);
    }
    
    result.data[col][row] = m.data[row][col];
    Matrix partial = recursive_transpose(m, row, col + 1);
    
    for (int i = 0; i < partial.rows; i++) {
        for (int j = 0; j < partial.cols; j++) {
            if (i == col && j == row) {
                continue;
            }
            result.data[i][j] = partial.data[i][j];
        }
    }
    
    return result;
}

Matrix recursive_multiply(Matrix a, Matrix b, int row, int col, int k) {
    Matrix result = create_matrix(a.rows, b.cols);
    
    if (row >= a.rows) {
        return result;
    }
    
    if (col >= b.cols) {
        return recursive_multiply(a, b, row + 1, 0, 0);
    }
    
    if (k >= a.cols) {
        Matrix partial = recursive_multiply(a, b, row, col + 1, 0);
        for (int i = 0; i < partial.rows; i++) {
            for (int j = 0; j < partial.cols; j++) {
                result.data[i][j] = partial.data[i][j];
            }
        }
        return result;
    }
    
    Matrix partial = recursive_multiply(a, b, row, col, k + 1);
    for (int i = 0; i < partial.rows; i++) {
        for (int j = 0; j < partial.cols; j++) {
            result.data[i][j] = partial.data[i][j];
        }
    }
    
    int product = a.data[row][k] * b.data[k][col];
    if ((product > 0 && result.data[row][col] > INT_MAX - product) ||
        (product < 0 && result.data[row][col] < INT_MIN - product)) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    result.data[row][col] += product;
    
    return result;
}

int main() {
    Matrix a, b;
    
    printf("Matrix A:\n");
    read_matrix_input(&a);
    
    printf("Matrix B:\n");
    read_matrix_input(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(a);
    
    printf("\nMatrix B:\n");
    print_matrix(b);
    
    printf("\nTranspose of A:\n");
    Matrix transposed = recursive_transpose(a, 0, 0);
    print_matrix(transposed);
    
    if (a.cols == b.rows) {
        printf("\nMatrix multiplication A * B:\n");
        Matrix multiplied = recursive_multiply(a, b, 0, 0, 0);
        print_matrix(multiplied);
    } else {
        printf("\nCannot multiply matrices: dimension mismatch\n");
    }
    
    return 0;
}