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

int read_int_safe(int min_val, int max_val) {
    int value;
    char buffer[32];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return INT_MIN;
    }
    
    if (sscanf(buffer, "%d", &value) != 1) {
        return INT_MIN;
    }
    
    if (value < min_val || value > max_val) {
        return INT_MIN;
    }
    
    return value;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%4d", m->data[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(Matrix *m) {
    printf("Enter number of rows (1-%d): ", MAX_DIM);
    m->rows = read_int_safe(1, MAX_DIM);
    if (m->rows == INT_MIN) {
        printf("Invalid rows input\n");
        exit(1);
    }
    
    printf("Enter number of columns (1-%d): ", MAX_DIM);
    m->cols = read_int_safe(1, MAX_DIM);
    if (m->cols == INT_MIN) {
        printf("Invalid columns input\n");
        exit(1);
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            int val = read_int_safe(INT_MIN + 1, INT_MAX - 1);
            if (val == INT_MIN) {
                printf("Invalid element input\n");
                exit(1);
            }
            m->data[i][j] = val;
        }
    }
}

int recursive_trace(const Matrix *m, int row, int col, int sum) {
    if (row >= m->rows || col >= m->cols) {
        return sum;
    }
    
    int new_sum;
    if (sum > 0 && m->data[row][col] > INT_MAX - sum) {
        new_sum = INT_MAX;
    } else if (sum < 0 && m->data[row][col] < INT_MIN - sum) {
        new_sum = INT_MIN;
    } else {
        new_sum = sum + m->data[row][col];
    }
    
    if (row == m->rows - 1 && col == m->cols - 1) {
        return new_sum;
    }
    
    if (row < m->rows - 1 && col < m->cols - 1) {
        int diag_sum = recursive_trace(m, row + 1, col + 1, new_sum);
        return diag_sum;
    } else if (row < m->rows - 1) {
        return recursive_trace(m, row + 1, col, new_sum);
    } else {
        return recursive_trace(m, row, col + 1, new_sum);
    }
}

int recursive_determinant(const Matrix *m, int size, int *used_cols, int row) {
    if (row == size) {
        return 1;
    }
    
    int det = 0;
    int sign = 1;
    
    for (int col = 0; col < size; col++) {
        if (used_cols[col]) {
            continue;
        }
        
        used_cols[col] = 1;
        
        int sub_det = recursive_determinant(m, size, used_cols, row + 1);
        
        int product;
        if (m->data[row][col] > 0 && sub_det > 0) {
            if (m->data[row][col] > INT_MAX / sub_det) {
                product = INT_MAX;
            } else {
                product = m->data[row][col] * sub_det;
            }
        } else if (m->data[row][col] < 0 && sub_det < 0) {
            if (m->data[row][col] < INT_MAX / sub_det) {
                product = m->data[row][col] * sub_det;
            } else {
                product = INT_MAX;
            }
        } else {
            product = m->data[row][col] * sub_det;
        }
        
        if (sign > 0) {
            if (det > INT_MAX - product) {
                det = INT_MAX;
            } else {
                det += product;
            }
        } else {
            if (det < INT_MIN - product) {
                det = INT_MIN;
            } else {
                det -= product;
            }
        }
        
        used_cols[col] = 0;
        sign = -sign;
    }
    
    return det;
}

int matrix_determinant(const Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for determinant\n");
        return 0;
    }
    
    int used_cols[MAX_DIM] = {0};
    return recursive_determinant(m, m->rows, used_cols