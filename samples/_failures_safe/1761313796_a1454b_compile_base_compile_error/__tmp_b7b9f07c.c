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
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_SIZE, MAX_SIZE);
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
        printf("Matrix must be square for trace calculation\n");
        return 0;
    }
    return recursive_trace(m, 0, 0, 0);
}

int recursive_determinant(const Matrix *m, int size, int temp[MAX_SIZE][MAX_SIZE]) {
    if (size == 1) {
        return temp[0][0];
    }
    
    int det = 0;
    int sign = 1;
    int submatrix[MAX_SIZE][MAX_SIZE];
    
    for (int col = 0; col < size; col++) {
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                submatrix[sub_i][sub_j] = temp[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        int minor = recursive_determinant(m, size - 1, submatrix);
        
        int term;
        if (sign > 0) {
            if (temp[0][col] > 0 && minor > INT_MAX / temp[0][col]) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            if (temp[0][col] < 0 && minor < INT_MIN / temp[0][col]) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            term = temp[0][col] * minor;
        } else {
            if (temp[0][col] > 0 && minor < INT_MIN / temp[0][col]) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            if (temp[0][col] < 0 && minor > INT_MAX / temp[0][col]) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            term = temp[0][col] * minor;
        }
        
        if (sign > 0) {
            if (det > INT_MAX - term) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            det += term;
        } else {
            if (det < INT_MIN - term) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            det += term;
        }
        
        sign = -sign;
    }
    
    return det;
}

int matrix_determinant(const Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for determinant calculation\n");
        return 0;
    }
    
    int temp[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            temp[i][j] = m->data[i][j];
        }
    }
    
    return recursive_determinant(m, m->rows, temp);
}

int main() {
    Matrix mat;
    
    printf("Matrix Operations Program\n");
    read_matrix(&mat);
    
    printf("\nMatrix:\n");
    print_matrix(&mat);
    
    printf("\nMatrix