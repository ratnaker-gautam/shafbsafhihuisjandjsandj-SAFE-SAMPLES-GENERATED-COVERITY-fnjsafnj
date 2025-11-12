//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

void read_matrix(Matrix *m) {
    printf("Enter rows and columns: ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        fprintf(stderr, "Invalid input\n");
        exit(1);
    }
    
    if (!validate_dimension(m->rows) || !validate_dimension(m->cols)) {
        fprintf(stderr, "Dimensions must be between 1 and %d\n", MAX_DIM);
        exit(1);
    }
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                fprintf(stderr, "Invalid matrix element\n");
                exit(1);
            }
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int recursive_trace(const Matrix *m, int row, int col, int sum) {
    if (row >= m->rows || col >= m->cols) {
        return sum;
    }
    
    if (sum > INT_MAX - m->data[row][col]) {
        fprintf(stderr, "Integer overflow detected\n");
        exit(1);
    }
    
    int new_sum = sum + m->data[row][col];
    
    if (row == m->rows - 1 && col == m->cols - 1) {
        return new_sum;
    }
    
    if (row < m->rows - 1) {
        return recursive_trace(m, row + 1, col, new_sum);
    } else {
        return recursive_trace(m, row, col + 1, new_sum);
    }
}

int calculate_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        fprintf(stderr, "Matrix must be square for trace calculation\n");
        exit(1);
    }
    return recursive_trace(m, 0, 0, 0);
}

Matrix recursive_transpose(const Matrix *m, Matrix *result, int row, int col) {
    if (row >= m->rows) {
        return *result;
    }
    
    if (col >= m->cols) {
        return recursive_transpose(m, result, row + 1, 0);
    }
    
    result->data[col][row] = m->data[row][col];
    return recursive_transpose(m, result, row, col + 1);
}

Matrix transpose_matrix(const Matrix *m) {
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
    
    if (mat.rows == mat.cols) {
        int trace = calculate_trace(&mat);
        printf("\nMatrix trace: %d\n", trace);
    }
    
    Matrix transposed = transpose_matrix(&mat);
    printf("\nTransposed matrix:\n");
    print_matrix(&transposed);
    
    return 0;
}