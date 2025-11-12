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
            printf("%3d ", m.data[i][j]);
        }
        printf("\n");
    }
}

Matrix recursive_transpose(Matrix m, int row, int col) {
    Matrix result = create_matrix(m.cols, m.rows);
    if (m.rows == 0 || m.cols == 0) {
        return result;
    }
    
    if (row >= m.rows) {
        return result;
    }
    
    if (col >= m.cols) {
        return recursive_transpose(m, row + 1, 0);
    }
    
    result = recursive_transpose(m, row, col + 1);
    result.data[col][row] = m.data[row][col];
    return result;
}

Matrix transpose_matrix(Matrix m) {
    return recursive_transpose(m, 0, 0);
}

int read_int_safe(const char* prompt, int min, int max) {
    char buffer[100];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            continue;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d\n", min, max);
    }
}

void fill_matrix(Matrix* m) {
    if (m->rows == 0 || m->cols == 0) {
        return;
    }
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Enter value for [%d][%d]: ", i, j);
            m->data[i][j] = read_int_safe("", -1000, 1000);
        }
    }
}

int main() {
    printf("Matrix Transpose using Recursion\n");
    printf("===============================\n");
    
    int rows = read_int_safe("Enter number of rows (1-10): ", 1, MAX_SIZE);
    int cols = read_int_safe("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    Matrix original = create_matrix(rows, cols);
    printf("\nFilling original matrix:\n");
    fill_matrix(&original);
    
    printf("\nOriginal matrix:\n");
    print_matrix(original);
    
    Matrix transposed = transpose_matrix(original);
    printf("\nTransposed matrix:\n");
    print_matrix(transposed);
    
    return 0;
}