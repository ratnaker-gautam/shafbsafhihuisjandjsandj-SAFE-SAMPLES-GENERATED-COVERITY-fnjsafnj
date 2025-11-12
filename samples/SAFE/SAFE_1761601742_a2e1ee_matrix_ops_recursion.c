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
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        printf("Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_size(m->rows, m->cols)) {
        printf("Invalid dimensions\n");
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
    
    int det = 0;
    int sign = 1;
    int temp[MAX_SIZE][MAX_SIZE];
    
    for (int col = 0; col < size; col++) {
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                temp[sub_i][sub_j] = mat[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        int sub_det = recursive_determinant(size - 1, temp);
        if (sub_det > 0 && mat[0][col] > INT_MAX / sub_det) {
            printf("Overflow detected\n");
            exit(1);
        }
        if (sub_det < 0 && mat[0][col] < INT_MIN / sub_det) {
            printf("Overflow detected\n");
            exit(1);
        }
        int term = sign * mat[0][col] * sub_det;
        
        if ((term > 0 && det > INT_MAX - term) || (term < 0 && det < INT_MIN - term)) {
            printf("Overflow detected\n");
            exit(1);
        }
        det += term;
        sign = -sign;
    }
    
    return det;
}

int matrix_determinant(const Matrix *m) {
    if (m->rows != m->cols) {
        printf("Matrix must be square for determinant\n");
        exit(1);
    }
    return recursive_determinant(m->rows, m->data);
}

void recursive_transpose(int size, int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int i, int j) {
    if (i >= size) return;
    if (j >= size) {
        recursive_transpose(size, src, dst, i + 1, 0);
        return;
    }
    dst[j][i] = src[i][j];
    recursive_transpose(size, src, dst, i, j + 1);
}

void matrix_transpose(const Matrix *src, Matrix *dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    recursive_transpose(src->rows, src->data, dst->data, 0, 0);
}

int main() {
    Matrix mat1, result;
    
    printf("Matrix Operations (Recursive Implementation)\n");
    printf("===========================================\n");
    
    read_matrix(&mat1);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat1);
    
    if (mat1.rows == mat1.cols) {
        int det = matrix_determinant(&mat1);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nMatrix is not square, skipping determinant\n");
    }
    
    printf("\nTransposed matrix:\n");
    matrix_transpose(&mat1, &result);
    print_matrix(&result);
    
    return 0;
}