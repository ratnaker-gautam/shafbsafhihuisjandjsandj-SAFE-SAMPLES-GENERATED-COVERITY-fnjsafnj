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

int validate_matrix_dimensions(int rows, int cols) {
    return (rows > 0 && rows <= MAX_DIM && cols > 0 && cols <= MAX_DIM);
}

void read_matrix(Matrix *m) {
    printf("Enter matrix dimensions (rows cols, max %dx%d): ", MAX_DIM, MAX_DIM);
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        printf("Invalid input\n");
        exit(1);
    }
    
    if (!validate_matrix_dimensions(m->rows, m->cols)) {
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

int recursive_matrix_sum(const Matrix *m, int row, int col) {
    if (row >= m->rows) return 0;
    
    int next_row = row;
    int next_col = col + 1;
    if (next_col >= m->cols) {
        next_row++;
        next_col = 0;
    }
    
    return m->data[row][col] + recursive_matrix_sum(m, next_row, next_col);
}

void recursive_transpose(const Matrix *src, Matrix *dst, int row, int col) {
    if (row >= src->rows) return;
    
    dst->data[col][row] = src->data[row][col];
    
    int next_row = row;
    int next_col = col + 1;
    if (next_col >= src->cols) {
        next_row++;
        next_col = 0;
    }
    
    if (next_row < src->rows) {
        recursive_transpose(src, dst, next_row, next_col);
    }
}

int recursive_determinant(const Matrix *m, int size, int temp[MAX_DIM][MAX_DIM]) {
    if (size == 1) return temp[0][0];
    if (size == 2) return temp[0][0] * temp[1][1] - temp[0][1] * temp[1][0];
    
    int det = 0;
    int sign = 1;
    int sub[MAX_DIM][MAX_DIM];
    
    for (int f = 0; f < size; f++) {
        int sub_i = 0;
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == f) continue;
                sub[sub_i][sub_j] = temp[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        int sub_det = recursive_determinant(m, size - 1, sub);
        if (sub_det > 0 && sign > 0 && det > INT_MAX - sub_det) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        if (sub_det < 0 && sign < 0 && det < INT_MIN - sub_det) {
            printf("Integer overflow detected\n");
            exit(1);
        }
        det += sign * temp[0][f] * sub_det;
        sign = -sign;
    }
    return det;
}

int main() {
    Matrix m1, m2;
    
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    read_matrix(&m1);
    
    printf("\nFirst matrix:\n");
    print_matrix(&m1);
    
    printf("\nMatrix sum: %d\n", recursive_matrix_sum(&m1, 0, 0));
    
    if (m1.rows == m1.cols) {
        int temp[MAX_DIM][MAX_DIM];
        for (int i = 0; i < m1.rows; i++) {
            for (int j = 0; j < m1.cols; j++) {
                temp[i][j] = m1.data[i][j];
            }
        }
        printf("Determinant: %d\n", recursive_determinant(&m1, m1.rows, temp));
    } else {
        printf("Determinant requires square matrix\n");
    }
    
    m2.rows = m1.cols;
    m2.cols = m1.rows;
    recursive_transpose(&m1, &m2, 0, 0);
    printf("\nTranspose:\n");
    print_matrix(&m2);
    
    return 0;
}