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

void print_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%d ", m.data[i][j]);
        }
        printf("\n");
    }
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
        Matrix sub_result = recursive_multiply(a, b, row, col + 1, 0);
        result.data[row][col] = result.data[row][col];
        for (int i = 0; i < a.rows; i++) {
            for (int j = 0; j < b.cols; j++) {
                result.data[i][j] = sub_result.data[i][j];
            }
        }
        return result;
    }
    
    result.data[row][col] += a.data[row][k] * b.data[k][col];
    Matrix sub_result = recursive_multiply(a, b, row, col, k + 1);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
            result.data[i][j] += sub_result.data[i][j];
        }
    }
    return result;
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    if (a.cols != b.rows) {
        printf("Error: Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    if (a.rows > MAX_SIZE || a.cols > MAX_SIZE || b.rows > MAX_SIZE || b.cols > MAX_SIZE) {
        printf("Error: Matrix size exceeds maximum\n");
        exit(1);
    }
    return recursive_multiply(a, b, 0, 0, 0);
}

int read_int() {
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        exit(1);
    }
    int value;
    if (sscanf(buffer, "%d", &value) != 1) {
        printf("Invalid input\n");
        exit(1);
    }
    return value;
}

Matrix read_matrix() {
    int rows, cols;
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    rows = read_int();
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    cols = read_int();
    
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        printf("Error: Invalid matrix dimensions\n");
        exit(1);
    }
    
    Matrix m = create_matrix(rows, cols);
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m.data[i][j] = read_int();
        }
    }
    return m;
}

int main() {
    printf("Matrix Multiplication using Recursion\n");
    printf("====================================\n");
    
    printf("First matrix:\n");
    Matrix a = read_matrix();
    
    printf("Second matrix:\n");
    Matrix b = read_matrix();
    
    printf("\nFirst matrix:\n");
    print_matrix(a);
    
    printf("\nSecond matrix:\n");
    print_matrix(b);
    
    printf("\nResult of multiplication:\n");
    Matrix result = multiply_matrices(a, b);
    print_matrix(result);
    
    return 0;
}