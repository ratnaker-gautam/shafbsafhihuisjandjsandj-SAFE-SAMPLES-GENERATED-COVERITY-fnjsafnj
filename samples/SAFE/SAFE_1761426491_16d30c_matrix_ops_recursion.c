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
    
    int new_sum;
    if (sum > 0 && m->data[row][col] > INT_MAX - sum) {
        return INT_MAX;
    }
    if (sum < 0 && m->data[row][col] < INT_MIN - sum) {
        return INT_MIN;
    }
    new_sum = sum + m->data[row][col];
    
    if (row == m->rows - 1 && col == m->cols - 1) {
        return new_sum;
    }
    
    if (row < m->rows - 1) {
        int down_sum = recursive_trace(m, row + 1, col, new_sum);
        if (down_sum != INT_MAX && down_sum != INT_MIN) {
            return down_sum;
        }
    }
    
    if (col < m->cols - 1) {
        int right_sum = recursive_trace(m, row, col + 1, new_sum);
        if (right_sum != INT_MAX && right_sum != INT_MIN) {
            return right_sum;
        }
    }
    
    return new_sum;
}

int calculate_matrix_trace(Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for trace calculation\n");
        return 0;
    }
    return recursive_trace(m, 0, 0, 0);
}

int main() {
    Matrix mat;
    
    printf("Matrix Operations - Recursive Trace Calculator\n");
    read_matrix(&mat);
    
    printf("\nInput matrix:\n");
    print_matrix(&mat);
    
    int trace = calculate_matrix_trace(&mat);
    if (trace == INT_MAX || trace == INT_MIN) {
        printf("Trace calculation overflow\n");
        return 1;
    }
    
    printf("Matrix trace: %d\n", trace);
    
    return 0;
}