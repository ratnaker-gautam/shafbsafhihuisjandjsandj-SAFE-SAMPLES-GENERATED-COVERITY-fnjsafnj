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
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    return 1;
}

void read_matrix(Matrix *m) {
    printf("Enter matrix rows and columns (max %d): ", MAX_SIZE);
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        printf("Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_size(m->rows, m->cols)) {
        printf("Invalid matrix size\n");
        exit(1);
    }
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

void print_matrix(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int recursive_trace(Matrix *m, int row, int col, int sum) {
    if (row >= m->rows || col >= m->cols) {
        return sum;
    }
    
    if (sum > INT_MAX - m->data[row][col]) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    
    return recursive_trace(m, row + 1, col + 1, sum + m->data[row][col]);
}

int calculate_trace(Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for trace calculation\n");
        return -1;
    }
    
    return recursive_trace(m, 0, 0, 0);
}

Matrix recursive_transpose(Matrix *m, Matrix *result, int row, int col) {
    if (row >= m->rows) {
        return *result;
    }
    
    if (col >= m->cols) {
        return recursive_transpose(m, result, row + 1, 0);
    }
    
    result->data[col][row] = m->data[row][col];
    return recursive_transpose(m, result, row, col + 1);
}

Matrix transpose_matrix(Matrix *m) {
    Matrix result;
    result.rows = m->cols;
    result.cols = m->rows;
    
    return recursive_transpose(m, &result, 0, 0);
}

int main() {
    Matrix mat;
    
    printf("Matrix Operations Program\n");
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    int trace = calculate_trace(&mat);
    if (trace != -1) {
        printf("\nMatrix trace: %d\n", trace);
    }
    
    if (mat.rows > 0 && mat.cols > 0) {
        Matrix transposed = transpose_matrix(&mat);
        printf("\nTransposed matrix:\n");
        print_matrix(&transposed);
    }
    
    return 0;
}