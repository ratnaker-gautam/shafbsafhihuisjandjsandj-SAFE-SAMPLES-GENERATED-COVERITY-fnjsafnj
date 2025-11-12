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
        printf("Invalid input\n");
        exit(1);
    }
    
    if (!validate_dimension(m->rows) || !validate_dimension(m->cols)) {
        printf("Dimensions must be between 1 and %d\n", MAX_DIM);
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
    
    if (row == col) {
        int new_sum;
        if (sum > 0 && m->data[row][col] > INT_MAX - sum) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        new_sum = sum + m->data[row][col];
        return recursive_trace(m, row + 1, col + 1, new_sum);
    }
    
    return recursive_trace(m, row + 1, col + 1, sum);
}

int matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for trace\n");
        return 0;
    }
    return recursive_trace(m, 0, 0, 0);
}

void recursive_transpose(const Matrix *src, Matrix *dst, int row, int col) {
    if (row >= src->rows) {
        return;
    }
    if (col >= src->cols) {
        recursive_transpose(src, dst, row + 1, 0);
        return;
    }
    
    dst->data[col][row] = src->data[row][col];
    recursive_transpose(src, dst, row, col + 1);
}

void matrix_transpose(const Matrix *src, Matrix *dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    recursive_transpose(src, dst, 0, 0);
}

int main() {
    Matrix mat1, mat2;
    
    printf("Matrix Operations (Recursive Implementation)\n");
    printf("Enter first matrix:\n");
    read_matrix(&mat1);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat1);
    
    if (mat1.rows == mat1.cols) {
        int trace = matrix_trace(&mat1);
        printf("\nMatrix trace: %d\n", trace);
    }
    
    printf("\nMatrix transpose:\n");
    matrix_transpose(&mat1, &mat2);
    print_matrix(&mat2);
    
    return 0;
}