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
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        m.rows = 0;
        m.cols = 0;
        return m;
    }
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
    if (m.rows == 0 || m.cols == 0) {
        printf("Invalid matrix\n");
        return;
    }
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%d ", m.data[i][j]);
        }
        printf("\n");
    }
}

Matrix recursive_multiply(Matrix a, Matrix b, int row, int col, int k) {
    Matrix result = create_matrix(a.rows, b.cols);
    if (a.cols != b.rows) {
        return result;
    }
    if (row >= a.rows) {
        return result;
    }
    if (col >= b.cols) {
        return recursive_multiply(a, b, row + 1, 0, 0);
    }
    if (k >= a.cols) {
        Matrix next = recursive_multiply(a, b, row, col + 1, 0);
        result.data[row][col] = result.data[row][col];
        for (int i = 0; i < a.rows; i++) {
            for (int j = 0; j < b.cols; j++) {
                result.data[i][j] = next.data[i][j];
            }
        }
        return result;
    }
    result.data[row][col] += a.data[row][k] * b.data[k][col];
    Matrix partial = recursive_multiply(a, b, row, col, k + 1);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
            result.data[i][j] += partial.data[i][j];
        }
    }
    return result;
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    if (a.rows == 0 || a.cols == 0 || b.rows == 0 || b.cols == 0) {
        return create_matrix(0, 0);
    }
    if (a.cols != b.rows) {
        return create_matrix(0, 0);
    }
    return recursive_multiply(a, b, 0, 0, 0);
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

void read_matrix_data(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            m->data[i][j] = read_int(prompt, -100, 100);
        }
    }
}

int main() {
    printf("Matrix Multiplication using Recursion\n");
    printf("Maximum matrix size: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    int rows1 = read_int("Enter rows for matrix A: ", 1, MAX_SIZE);
    int cols1 = read_int("Enter columns for matrix A: ", 1, MAX_SIZE);
    Matrix a = create_matrix(rows1, cols1);
    printf("Enter elements for matrix A:\n");
    read_matrix_data(&a);
    
    int rows2 = read_int("Enter rows for matrix B: ", 1, MAX_SIZE);
    int cols2 = read_int("Enter columns for matrix B: ", 1, MAX_SIZE);
    Matrix b = create_matrix(rows2, cols2);
    printf("Enter elements for matrix B:\n");
    read_matrix_data(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(a);
    printf("\nMatrix B:\n");
    print_matrix(b);
    
    Matrix result = multiply_matrices(a, b);
    if (result.rows == 0 || result.cols == 0) {
        printf("\nMatrix multiplication failed: dimension mismatch or invalid matrices\n");
    } else {
        printf("\nResult (A x B):\n");
        print_matrix(result);
    }
    
    return 0;
}