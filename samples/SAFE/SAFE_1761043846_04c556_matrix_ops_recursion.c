//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
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
    if (m.rows <= 0 || m.cols <= 0) {
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
    if (m.rows <= 0 || m.cols <= 0) {
        return create_matrix(0, 0);
    }
    return recursive_transpose(m, 0, 0);
}

int recursive_trace(Matrix m, int diag, int sum) {
    if (diag >= m.rows || diag >= m.cols) {
        return sum;
    }
    return recursive_trace(m, diag + 1, sum + m.data[diag][diag]);
}

int matrix_trace(Matrix m) {
    if (m.rows <= 0 || m.cols <= 0 || m.rows != m.cols) {
        return 0;
    }
    return recursive_trace(m, 0, 0);
}

int read_int(const char* prompt, int min, int max) {
    char buffer[100];
    int value;
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

void fill_matrix(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            m->data[i][j] = read_int(prompt, -100, 100);
        }
    }
}

int main() {
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    int cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    Matrix m = create_matrix(rows, cols);
    printf("\nFilling matrix %dx%d:\n", rows, cols);
    fill_matrix(&m);
    
    printf("\nOriginal matrix:\n");
    print_matrix(m);
    
    printf("\nMatrix trace: %d\n", matrix_trace(m));
    
    Matrix transposed = transpose_matrix(m);
    printf("\nTransposed matrix:\n");
    print_matrix(transposed);
    
    return 0;
}