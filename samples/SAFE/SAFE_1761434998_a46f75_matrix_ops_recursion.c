//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void recursive_matrix_fill(Matrix *mat, int row, int col, int value) {
    if (row >= mat->rows) return;
    if (col >= mat->cols) {
        recursive_matrix_fill(mat, row + 1, 0, value);
        return;
    }
    mat->data[row][col] = value + row + col;
    recursive_matrix_fill(mat, row, col + 1, value);
}

void recursive_matrix_print(const Matrix *mat, int row, int col) {
    if (row >= mat->rows) {
        printf("\n");
        return;
    }
    if (col >= mat->cols) {
        printf("\n");
        recursive_matrix_print(mat, row + 1, 0);
        return;
    }
    printf("%4d", mat->data[row][col]);
    recursive_matrix_print(mat, row, col + 1);
}

int recursive_matrix_trace(const Matrix *mat, int row, int sum) {
    if (row >= mat->rows || row >= mat->cols) return sum;
    return recursive_matrix_trace(mat, row + 1, sum + mat->data[row][row]);
}

int recursive_matrix_sum(const Matrix *mat, int row, int col, int sum) {
    if (row >= mat->rows) return sum;
    if (col >= mat->cols) {
        return recursive_matrix_sum(mat, row + 1, 0, sum);
    }
    return recursive_matrix_sum(mat, row, col + 1, sum + mat->data[row][col]);
}

int main() {
    Matrix mat;
    int rows, cols;
    
    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter matrix cols (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_matrix_size(rows, cols)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    mat.rows = rows;
    mat.cols = cols;
    
    recursive_matrix_fill(&mat, 0, 0, 1);
    
    printf("Generated matrix:\n");
    recursive_matrix_print(&mat, 0, 0);
    
    if (rows == cols) {
        int trace = recursive_matrix_trace(&mat, 0, 0);
        printf("Matrix trace: %d\n", trace);
    }
    
    int total_sum = recursive_matrix_sum(&mat, 0, 0, 0);
    printf("Matrix sum: %d\n", total_sum);
    
    return 0;
}