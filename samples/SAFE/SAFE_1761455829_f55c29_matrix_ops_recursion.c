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

int recursive_determinant(int size, int mat[MAX_SIZE][MAX_SIZE]) {
    if (size == 1) {
        return mat[0][0];
    }
    
    if (size == 2) {
        int64_t det = (int64_t)mat[0][0] * mat[1][1] - (int64_t)mat[0][1] * mat[1][0];
        if (det > INT_MAX || det < INT_MIN) {
            printf("Determinant overflow\n");
            exit(1);
        }
        return (int)det;
    }
    
    int det = 0;
    int sign = 1;
    
    for (int col = 0; col < size; col++) {
        int submat[MAX_SIZE][MAX_SIZE];
        int sub_i = 0;
        
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j != col) {
                    submat[sub_i][sub_j] = mat[i][j];
                    sub_j++;
                }
            }
            sub_i++;
        }
        
        int64_t term = (int64_t)sign * mat[0][col] * recursive_determinant(size - 1, submat);
        if (term > INT_MAX || term < INT_MIN) {
            printf("Determinant overflow\n");
            exit(1);
        }
        
        det += (int)term;
        sign = -sign;
    }
    
    return det;
}

int matrix_determinant(const Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for determinant\n");
        exit(1);
    }
    
    if (m->rows == 0 || m->rows > MAX_SIZE) {
        printf("Invalid matrix size\n");
        exit(1);
    }
    
    return recursive_determinant(m->rows, m->data);
}

int main() {
    Matrix mat;
    
    printf("Matrix Determinant Calculator\n");
    read_matrix(&mat);
    
    printf("\nInput matrix:\n");
    print_matrix(&mat);
    
    if (mat.rows == mat.cols) {
        int det = matrix_determinant(&mat);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nMatrix is not square, cannot compute determinant\n");
    }
    
    return 0;
}