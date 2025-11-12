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

void print_matrix(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
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
        
        determinant += sign * (int)term;
        sign = -sign;
    }
    
    return determinant;
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

int main() {
    Matrix mat, transposed;
    
    printf("Matrix Operations using Recursion\n");
    printf("Maximum matrix size: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    read_matrix(&mat);
    
    printf("\nOriginal matrix:\n");
    print_matrix(&mat);
    
    if (mat.rows == mat.cols) {
        int det = recursive_determinant(&mat, mat.rows);
        printf("\nDeterminant: %d\n", det);
    } else {
        printf("\nDeterminant only defined for square matrices\n");
    }
    
    transposed.rows = mat.cols;
    transposed.cols = mat.rows;
    
    printf("\nComputing transpose...\n");
    recursive_transpose(&mat, &transposed, 0, 0);
    
    printf("Transposed matrix:\n");
    print_matrix(&transposed);
    
    return 0;
}