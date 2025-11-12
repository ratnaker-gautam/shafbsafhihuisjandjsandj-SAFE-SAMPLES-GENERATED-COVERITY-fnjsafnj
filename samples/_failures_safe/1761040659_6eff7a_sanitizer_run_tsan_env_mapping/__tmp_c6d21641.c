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
        
        int64_t minor = (int64_t)m->data[0][col] * recursive_determinant(&submatrix, size - 1);
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

int main() {
    Matrix mat;
    
    printf("Matrix Determinant Calculator\n");
    read_matrix(&mat);
    
    if (mat.rows != mat.cols) {
        printf("Determinant only defined for square matrices\n");
        return 1;
    }
    
    printf("\nInput matrix:\n");
    print_matrix(&mat);
    
    int det = recursive_determinant(&mat, mat.rows);
    printf("\nDeterminant: %d\n", det);
    
    return 0;
}