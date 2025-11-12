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

void fill_matrix_recursive(Matrix *m, int row, int col, int value) {
    if (row >= m->rows || col >= m->cols) return;
    
    m->data[row][col] = value;
    
    if (col + 1 < m->cols) {
        fill_matrix_recursive(m, row, col + 1, value + 1);
    } else if (row + 1 < m->rows) {
        fill_matrix_recursive(m, row + 1, 0, value + 1);
    }
}

Matrix multiply_matrices_recursive(Matrix a, Matrix b, int row, int col, int k, int sum) {
    Matrix result = create_matrix(a.rows, b.cols);
    
    if (row >= a.rows || col >= b.cols) return result;
    
    if (k < a.cols) {
        sum += a.data[row][k] * b.data[k][col];
        return multiply_matrices_recursive(a, b, row, col, k + 1, sum);
    }
    
    result.data[row][col] = sum;
    
    if (col + 1 < b.cols) {
        Matrix next = multiply_matrices_recursive(a, b, row, col + 1, 0, 0);
        for (int i = 0; i < result.rows; i++) {
            for (int j = 0; j < result.cols; j++) {
                if (i == row && j == col) continue;
                result.data[i][j] = next.data[i][j];
            }
        }
    } else if (row + 1 < a.rows) {
        Matrix next = multiply_matrices_recursive(a, b, row + 1, 0, 0, 0);
        for (int i = 0; i < result.rows; i++) {
            for (int j = 0; j < result.cols; j++) {
                if (i == row && j == col) continue;
                result.data[i][j] = next.data[i][j];
            }
        }
    }
    
    return result;
}

int get_valid_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d.\n", min, max);
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication using Recursion\n");
    printf("=====================================\n");
    
    printf("First matrix dimensions:\n");
    rows1 = get_valid_int("Rows (1-10): ", 1, MAX_SIZE);
    cols1 = get_valid_int("Columns (1-10): ", 1, MAX_SIZE);
    
    printf("Second matrix dimensions:\n");
    rows2 = get_valid_int("Rows (1-10): ", 1, MAX_SIZE);
    cols2 = get_valid_int("Columns (1-10): ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Error: Cannot multiply matrices. Columns of first must equal rows of second.\n");
        return 1;
    }
    
    Matrix mat1 = create_matrix(rows1, cols1);
    Matrix mat2 = create_matrix(rows2, cols2);
    
    fill_matrix_recursive(&mat1, 0, 0, 1);
    fill_matrix_recursive(&mat2, 0, 0, 1);
    
    printf("\nFirst matrix:\n");
    print_matrix(mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(mat2);
    
    Matrix result = multiply_matrices_recursive(mat1, mat2, 0, 0, 0, 0);
    
    printf("\nResult matrix:\n");
    print_matrix(result);
    
    return 0;
}