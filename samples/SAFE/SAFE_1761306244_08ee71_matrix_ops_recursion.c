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
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        printf("Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_size(m->rows, m->cols)) {
        printf("Invalid matrix size\n");
        exit(1);
    }
    
    printf("Enter matrix elements row by row:\n");
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
    if (row >= m->rows || col >= m->cols) return sum;
    
    int new_sum;
    if (sum > 0 && m->data[row][col] > INT_MAX - sum) return INT_MAX;
    if (sum < 0 && m->data[row][col] < INT_MIN - sum) return INT_MIN;
    new_sum = sum + m->data[row][col];
    
    if (row == m->rows - 1 && col == m->cols - 1) return new_sum;
    
    if (row < m->rows - 1 && col < m->cols - 1) {
        int diag = recursive_trace(m, row + 1, col + 1, new_sum);
        return diag;
    } else if (row < m->rows - 1) {
        return recursive_trace(m, row + 1, col, new_sum);
    } else {
        return recursive_trace(m, row, col + 1, new_sum);
    }
}

int calculate_trace_sum(Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for diagonal trace\n");
        return -1;
    }
    return recursive_trace(m, 0, 0, 0);
}

int main() {
    Matrix mat;
    
    printf("Matrix Diagonal Trace Calculator\n");
    printf("===============================\n");
    
    read_matrix(&mat);
    
    printf("\nInput matrix:\n");
    print_matrix(&mat);
    
    int trace = calculate_trace_sum(&mat);
    if (trace != -1) {
        printf("\nDiagonal trace sum: %d\n", trace);
    }
    
    return 0;
}