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

void read_matrix(Matrix *m) {
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        fprintf(stderr, "Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_size(m->rows, m->cols)) {
        fprintf(stderr, "Invalid matrix size\n");
        exit(1);
    }
    
    printf("Enter matrix elements row by row:\n");
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

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        fprintf(stderr, "Integer overflow detected\n");
        exit(1);
    }
    return a + b;
}

int recursive_trace(const Matrix *m, int row, int col, int sum) {
    if (row >= m->rows || col >= m->cols) {
        return sum;
    }
    
    if (row == m->rows - 1 && col == m->cols - 1) {
        return safe_add(sum, m->data[row][col]);
    }
    
    int current_sum = safe_add(sum, m->data[row][col]);
    
    if (row < m->rows - 1) {
        return recursive_trace(m, row + 1, col, current_sum);
    } else {
        return recursive_trace(m, row, col + 1, current_sum);
    }
}

int calculate_matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        fprintf(stderr, "Trace requires square matrix\n");
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

Matrix matrix_transpose(const Matrix *m) {
    Matrix result;
    result.rows = m->cols;
    result.cols = m->rows;
    
    if (!validate_matrix_size(result.rows, result.cols)) {
        fprintf(stderr, "Transpose would exceed maximum size\n");
        exit(1);
    }
    
    return recursive_transpose(m, &result, 0, 0);
}

int main() {
    Matrix mat;
    
    printf("Matrix Operations Program\n");
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    if (mat.rows == mat.cols) {
        int trace = calculate_matrix_trace(&mat);
        printf("\nMatrix trace: %d\n", trace);
    }
    
    printf("\nMatrix transpose:\n");
    Matrix transposed = matrix_transpose(&mat);
    print_matrix(&transposed);
    
    return 0;
}