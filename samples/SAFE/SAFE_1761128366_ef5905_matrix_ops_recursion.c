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
        fprintf(stderr, "Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_size(m->rows, m->cols)) {
        fprintf(stderr, "Invalid matrix size\n");
        exit(1);
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                fprintf(stderr, "Invalid matrix element\n");
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

int recursive_determinant(int n, int mat[MAX_SIZE][MAX_SIZE]) {
    if (n == 1) {
        return mat[0][0];
    }
    
    int det = 0;
    int sign = 1;
    int temp[MAX_SIZE][MAX_SIZE];
    
    for (int col = 0; col < n; col++) {
        int sub_i = 0;
        for (int i = 1; i < n; i++) {
            int sub_j = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                temp[sub_i][sub_j] = mat[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        int sub_det = recursive_determinant(n - 1, temp);
        if (sub_det > 0 && mat[0][col] > INT_MAX / sub_det) {
            fprintf(stderr, "Integer overflow detected\n");
            exit(1);
        }
        if (sub_det < 0 && mat[0][col] < INT_MIN / sub_det) {
            fprintf(stderr, "Integer overflow detected\n");
            exit(1);
        }
        int term = sign * mat[0][col] * sub_det;
        
        if ((term > 0 && det > INT_MAX - term) || (term < 0 && det < INT_MIN - term)) {
            fprintf(stderr, "Integer overflow detected\n");
            exit(1);
        }
        det += term;
        sign = -sign;
    }
    
    return det;
}

int calculate_determinant(const Matrix *m) {
    if (m->rows != m->cols) {
        fprintf(stderr, "Matrix must be square for determinant\n");
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
        int det = calculate_determinant(&mat);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nMatrix is not square, cannot compute determinant\n");
    }
    
    return 0;
}