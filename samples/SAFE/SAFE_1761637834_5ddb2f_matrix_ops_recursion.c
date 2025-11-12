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

int recursive_determinant(Matrix *m, int size) {
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
        
        int64_t term = (int64_t)m->data[0][col] * recursive_determinant(&submatrix, size - 1);
        if (term > INT_MAX || term < INT_MIN) {
            printf("Determinant overflow\n");
            exit(1);
        }
        
        int64_t result = (int64_t)determinant + sign * term;
        if (result > INT_MAX || result < INT_MIN) {
            printf("Determinant overflow\n");
            exit(1);
        }
        
        determinant = (int)result;
        sign = -sign;
    }
    
    return determinant;
}

int calculate_determinant(Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for determinant\n");
        return 0;
    }
    
    if (m->rows > MAX_SIZE) {
        printf("Matrix too large for determinant calculation\n");
        return 0;
    }
    
    return recursive_determinant(m, m->rows);
}

void recursive_transpose(Matrix *src, Matrix *dst, int row, int col) {
    if (row >= src->rows) return;
    
    if (col >= src->cols) {
        recursive_transpose(src, dst, row + 1, 0);
        return;
    }
    
    dst->data[col][row] = src->data[row][col];
    recursive_transpose(src, dst, row, col + 1);
}

void transpose_matrix(Matrix *src, Matrix *dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    recursive_transpose(src, dst, 0, 0);
}

int main() {
    Matrix mat, transposed;
    
    printf("Matrix Operations Program\n");
    printf("Maximum matrix size: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    if (mat.rows == mat.cols) {
        int det = calculate_determinant(&mat);
        if (mat.rows == mat.cols) {
            printf("\nDeterminant: %d\n", det);
        }
    }
    
    transpose_matrix(&mat, &transposed);
    printf("\nTransposed matrix:\n");
    print_matrix(&transposed);
    
    return 0;
}