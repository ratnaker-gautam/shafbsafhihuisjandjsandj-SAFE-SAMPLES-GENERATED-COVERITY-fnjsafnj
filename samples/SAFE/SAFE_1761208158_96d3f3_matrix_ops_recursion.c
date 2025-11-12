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

int calculate_diagonal_sum(Matrix *m, int index, int sum) {
    if (index >= m->rows || index >= m->cols) {
        return sum;
    }
    
    if (sum > INT_MAX - m->data[index][index]) {
        printf("Integer overflow detected\n");
        exit(1);
    }
    
    return calculate_diagonal_sum(m, index + 1, sum + m->data[index][index]);
}

int main() {
    Matrix mat;
    
    printf("Matrix Operations using Recursion\n");
    printf("================================\n");
    
    read_matrix(&mat);
    
    printf("\nEntered matrix:\n");
    print_matrix(&mat);
    
    if (mat.rows == mat.cols) {
        int diagonal_sum = calculate_diagonal_sum(&mat, 0, 0);
        printf("\nDiagonal sum: %d\n", diagonal_sum);
    } else {
        printf("\nMatrix is not square, skipping diagonal calculation\n");
    }
    
    int trace_sum = recursive_trace(&mat, 0, 0, 0);
    printf("Path sum (top-left to bottom-right): %d\n", trace_sum);
    
    return 0;
}