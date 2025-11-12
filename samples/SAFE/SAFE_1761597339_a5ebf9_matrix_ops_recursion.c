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
    printf("Enter matrix dimensions (rows cols): ");
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
    if (row >= m->rows || col >= m->cols) {
        return sum;
    }
    
    if (row == m->rows - 1 && col == m->cols - 1) {
        return sum + m->data[row][col];
    }
    
    int current_sum = sum + m->data[row][col];
    
    if (row < m->rows - 1 && col < m->cols - 1) {
        int diag_sum = recursive_trace(m, row + 1, col + 1, current_sum);
        int right_sum = recursive_trace(m, row, col + 1, current_sum);
        int down_sum = recursive_trace(m, row + 1, col, current_sum);
        
        return diag_sum > right_sum ? (diag_sum > down_sum ? diag_sum : down_sum) : 
               (right_sum > down_sum ? right_sum : down_sum);
    } else if (row < m->rows - 1) {
        return recursive_trace(m, row + 1, col, current_sum);
    } else {
        return recursive_trace(m, row, col + 1, current_sum);
    }
}

int find_max_path_sum(Matrix *m) {
    if (m->rows == 0 || m->cols == 0) {
        return 0;
    }
    return recursive_trace(m, 0, 0, 0);
}

int main() {
    Matrix mat;
    
    printf("Matrix Maximum Path Sum Calculator\n");
    printf("Maximum matrix size: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    read_matrix(&mat);
    
    printf("\nInput matrix:\n");
    print_matrix(&mat);
    
    int max_sum = find_max_path_sum(&mat);
    printf("\nMaximum path sum: %d\n", max_sum);
    
    return 0;
}