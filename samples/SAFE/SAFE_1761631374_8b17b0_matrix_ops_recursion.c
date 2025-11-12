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

int read_int(const char* prompt, int min, int max) {
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
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        break;
    }
    return value;
}

void read_matrix_elements(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            m->data[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

int recursive_trace(Matrix m, int row, int col, int sum) {
    if (row >= m.rows || col >= m.cols) {
        return sum;
    }
    if (m.rows != m.cols) {
        return -1;
    }
    if (row == m.rows - 1 && col == m.cols - 1) {
        return sum + m.data[row][col];
    }
    if (row == col) {
        sum += m.data[row][col];
    }
    return recursive_trace(m, row + 1, col + 1, sum);
}

int calculate_trace(Matrix m) {
    return recursive_trace(m, 0, 0, 0);
}

Matrix recursive_transpose(Matrix m, Matrix result, int row, int col) {
    if (row >= m.rows) {
        return result;
    }
    if (col >= m.cols) {
        return recursive_transpose(m, result, row + 1, 0);
    }
    result.data[col][row] = m.data[row][col];
    return recursive_transpose(m, result, row, col + 1);
}

Matrix transpose_matrix(Matrix m) {
    Matrix result = create_matrix(m.cols, m.rows);
    return recursive_transpose(m, result, 0, 0);
}

int main() {
    printf("Matrix Operations Program\n");
    
    int rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    int cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    Matrix m = create_matrix(rows, cols);
    read_matrix_elements(&m);
    
    printf("\nOriginal matrix:\n");
    print_matrix(m);
    
    if (rows == cols) {
        int trace = calculate_trace(m);
        printf("\nTrace of the matrix: %d\n", trace);
    } else {
        printf("\nMatrix is not square, cannot calculate trace.\n");
    }
    
    Matrix transposed = transpose_matrix(m);
    printf("\nTransposed matrix:\n");
    print_matrix(transposed);
    
    return 0;
}