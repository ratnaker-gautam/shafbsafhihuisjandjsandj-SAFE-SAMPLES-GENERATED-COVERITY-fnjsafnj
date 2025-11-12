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
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void print_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%d ", m.data[i][j]);
        }
        printf("\n");
    }
}

Matrix add_matrices_recursive(Matrix a, Matrix b, int row, int col) {
    if (row >= a.rows) {
        return a;
    }
    
    Matrix result = a;
    result.data[row][col] = a.data[row][col] + b.data[row][col];
    
    if (col + 1 < a.cols) {
        return add_matrices_recursive(result, b, row, col + 1);
    } else {
        return add_matrices_recursive(result, b, row + 1, 0);
    }
}

Matrix add_matrices(Matrix a, Matrix b) {
    if (a.rows != b.rows || a.cols != b.cols) {
        Matrix empty = create_matrix(0, 0);
        return empty;
    }
    return add_matrices_recursive(a, b, 0, 0);
}

void read_matrix_input(Matrix *m) {
    printf("Enter number of rows: ");
    if (scanf("%d", &m->rows) != 1) {
        m->rows = 0;
        m->cols = 0;
        return;
    }
    
    printf("Enter number of columns: ");
    if (scanf("%d", &m->cols) != 1) {
        m->rows = 0;
        m->cols = 0;
        return;
    }
    
    if (!validate_matrix_size(m->rows, m->cols)) {
        m->rows = 0;
        m->cols = 0;
        return;
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                m->rows = 0;
                m->cols = 0;
                return;
            }
        }
    }
}

int main() {
    Matrix matrix1, matrix2;
    
    printf("Matrix Addition Program\n");
    printf("Maximum matrix size: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    printf("\nFirst matrix:\n");
    read_matrix_input(&matrix1);
    if (matrix1.rows == 0 || matrix1.cols == 0) {
        printf("Invalid first matrix input\n");
        return 1;
    }
    
    printf("\nSecond matrix:\n");
    read_matrix_input(&matrix2);
    if (matrix2.rows == 0 || matrix2.cols == 0) {
        printf("Invalid second matrix input\n");
        return 1;
    }
    
    if (matrix1.rows != matrix2.rows || matrix1.cols != matrix2.cols) {
        printf("Matrices must have the same dimensions for addition\n");
        return 1;
    }
    
    Matrix result = add_matrices(matrix1, matrix2);
    
    printf("\nFirst matrix:\n");
    print_matrix(matrix1);
    
    printf("\nSecond matrix:\n");
    print_matrix(matrix2);
    
    printf("\nResult matrix:\n");
    print_matrix(result);
    
    return 0;
}