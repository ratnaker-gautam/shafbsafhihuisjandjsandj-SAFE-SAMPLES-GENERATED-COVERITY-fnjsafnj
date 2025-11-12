//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
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

int recursive_determinant(Matrix *m, int size) {
    if (size == 1) {
        return m->data[0][0];
    }
    
    if (size == 2) {
        return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
    }
    
    int det = 0;
    int sign = 1;
    
    for (int col = 0; col < size; col++) {
        Matrix submatrix;
        submatrix.rows = size - 1;
        submatrix.cols = size - 1;
        
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j != col) {
                    submatrix.data[sub_i][sub_j] = m->data[i][j];
                    sub_j++;
                }
            }
            sub_i++;
        }
        
        int sub_det = recursive_determinant(&submatrix, size - 1);
        if (m->data[0][col] > 0 && sub_det > INT_MAX / m->data[0][col]) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        if (m->data[0][col] < 0 && sub_det < INT_MIN / m->data[0][col]) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        det += sign * m->data[0][col] * sub_det;
        sign = -sign;
    }
    
    return det;
}

int calculate_determinant(Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for determinant\n");
        return 0;
    }
    return recursive_determinant(m, m->rows);
}

int main() {
    Matrix mat;
    
    printf("Matrix Operations Program\n");
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    printf("\nMaximum path sum: %d\n", find_max_path_sum(&mat));
    
    if (mat.rows == mat.cols) {
        printf("Determinant: %d\n", calculate_determinant(&mat));
    } else {
        printf("Matrix is not square, skipping determinant\n");