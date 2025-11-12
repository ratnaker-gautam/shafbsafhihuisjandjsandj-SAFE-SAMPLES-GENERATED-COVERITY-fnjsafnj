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
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        break;
    }
    return value;
}

void fill_matrix(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter value for [%d][%d]: ", i, j);
            m->data[i][j] = get_valid_int(prompt, -100, 100);
        }
    }
}

int main() {
    printf("Matrix Operations - Recursive Transpose\n");
    printf("======================================\n");
    
    int rows = get_valid_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    int cols = get_valid_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    Matrix original = create_matrix(rows, cols);
    printf("\nFilling matrix %dx%d:\n", rows, cols);
    fill_matrix(&original);
    
    printf("\nOriginal matrix:\n");
    print_matrix(original);
    
    Matrix transposed = recursive_transpose(original, 0, 0);
    printf("\nTransposed matrix:\n");
    print_matrix(transposed);
    
    return 0;
}