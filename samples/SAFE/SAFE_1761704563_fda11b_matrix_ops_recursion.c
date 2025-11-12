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
    
    sum += m->data[row][col];
    
    if (row == m->rows - 1 && col == m->cols - 1) {
        return sum;
    }
    
    if (row < m->rows - 1) {
        return recursive_trace(m, row + 1, col, sum);
    } else {
        return recursive_trace(m, row, col + 1, sum);
    }
}

int recursive_determinant(Matrix *m, int size, int *temp) {
    if (size == 1) {
        return m->data[0][0];
    }
    
    int det = 0;
    int sign = 1;
    
    for (int col = 0; col < size; col++) {
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j != col) {
                    temp[sub_i * (size - 1) + sub_j] = m->data[i][j];
                    sub_j++;
                }
            }
            sub_i++;
        }
        
        Matrix submatrix;
        submatrix.rows = size - 1;
        submatrix.cols = size - 1;
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - 1; j++) {
                submatrix.data[i][j] = temp[i * (size - 1) + j];
            }
        }
        
        int subdet = recursive_determinant(&submatrix, size - 1, temp + (size - 1) * (size - 1));
        
        if (m->data[0][col] > 0 && subdet > INT_MAX / m->data[0][col]) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        if (m->data[0][col] < 0 && subdet < INT_MIN / m->data[0][col]) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        
        int term = m->data[0][col] * subdet;
        
        if (sign > 0 && term > INT_MAX - det) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        if (sign < 0 && term < INT_MIN + det) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        
        det += sign * term;
        sign = -sign;
    }
    
    return det;
}

int main() {
    Matrix mat;
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    if (mat.rows == mat.cols) {
        int temp_buffer[MAX_SIZE * MAX_SIZE];
        int det = recursive_determinant(&mat, mat.rows, temp_buffer);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nMatrix is not square, cannot compute determinant\n");
    }
    
    int trace_sum = recursive_trace(&mat, 0, 0, 0);
    printf("Diagonal trace sum: %d\n", trace_sum);
    
    return 0;
}