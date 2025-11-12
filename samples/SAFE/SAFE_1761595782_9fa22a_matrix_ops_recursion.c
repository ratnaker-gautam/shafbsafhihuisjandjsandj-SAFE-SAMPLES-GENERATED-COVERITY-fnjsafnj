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

int recursive_trace(Matrix m, int diag, int sum) {
    if (diag >= m.rows || diag >= m.cols) {
        return sum;
    }
    return recursive_trace(m, diag + 1, sum + m.data[diag][diag]);
}

int validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        return 0;
    }
    if (rows > MAX_SIZE || cols > MAX_SIZE) {
        return 0;
    }
    return 1;
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void fill_matrix(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            m->data[i][j] = read_int("", -1000, 1000);
        }
    }
}

int main() {
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    int cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    Matrix m = create_matrix(rows, cols);
    printf("\nFill the matrix:\n");
    fill_matrix(&m);
    
    printf("\nOriginal matrix:\n");
    print_matrix(m);
    
    if (rows == cols) {
        int trace = recursive_trace(m, 0, 0);
        printf("\nTrace of matrix: %d\n", trace);
    } else {
        printf("\nMatrix is not square - cannot compute trace.\n");
    }
    
    printf("\nTranspose of matrix:\n");
    Matrix transposed = recursive_transpose(m, 0, 0);
    print_matrix(transposed);
    
    return 0;
}