//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: matrix_ops
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

void read_matrix(Matrix *m) {
    printf("Enter number of rows (1-%d): ", MAX_DIM);
    m->rows = read_int_safe(1, MAX_DIM);
    if (m->rows == INT_MIN) {
        printf("Invalid input for rows\n");
        exit(1);
    }
    
    printf("Enter number of columns (1-%d): ", MAX_DIM);
    m->cols = read_int_safe(1, MAX_DIM);
    if (m->cols == INT_MIN) {
        printf("Invalid input for columns\n");
        exit(1);
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            int val = read_int_safe(INT_MIN + 1, INT_MAX - 1);
            if (val == INT_MIN) {
                printf("Invalid input for element\n");
                exit(1);
            }
            m->data[i][j] = val;
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_determinant_recursive(const Matrix *m, int size) {
    if (size == 1) {
        return m->data[0][0];
    }
    
    if (size == 2) {
        int64_t det = (int64_t)m->data[0][0] * m->data[1][1] - 
                     (int64_t)m->data[0][1] * m->data[1][0];
        if (det > INT_MAX || det < INT_MIN) {
            printf("Determinant overflow\n");
            exit(1);
        }
        return (int)det;
    }
    
    int determinant = 0;
    int sign = 1;
    
    for (int col = 0; col < size; col++) {
        Matrix submatrix;
        submatrix.rows = size - 1;
        submatrix.cols = size - 1;
        
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                submatrix.data[sub_i][sub_j] = m->data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        int64_t minor = (int64_t)m->data[0][col] * matrix_determinant_recursive(&submatrix, size - 1);
        int64_t term = sign * minor;
        
        if (term > INT_MAX || term < INT_MIN) {
            printf("Determinant overflow\n");
            exit(1);
        }
        
        int64_t new_det = (int64_t)determinant + term;
        if (new_det > INT_MAX || new_det < INT_MIN) {
            printf("Determinant overflow\n");
            exit(1);
        }
        
        determinant = (int)new_det;
        sign = -sign;
    }
    
    return determinant;
}

int matrix_determinant(const Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for determinant calculation\n");
        exit(1);
    }
    
    if (m->rows > MAX_DIM) {
        printf("Matrix too large for determinant calculation\n");
        exit(1);
    }
    
    return matrix_determinant_recursive(m, m->rows);
}

void matrix_transpose_recursive(Matrix *result, const Matrix *m, int row, int col) {
    if (row >= m->rows) {
        return;
    }
    
    if (col >= m->cols) {
        matrix_transpose_recursive(result, m, row + 1, 0);
        return;
    }
    
    result->data[col][row] = m->data[row][col];
    matrix_transpose_recursive(result, m, row, col + 1);
}

Matrix matrix_transpose(const Matrix *m) {
    Matrix result;
    result.rows = m->cols;
    result.cols = m->rows;
    
    matrix_transpose_recursive(&result, m, 0, 0);
    return result;
}

int main() {
    Matrix mat;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n