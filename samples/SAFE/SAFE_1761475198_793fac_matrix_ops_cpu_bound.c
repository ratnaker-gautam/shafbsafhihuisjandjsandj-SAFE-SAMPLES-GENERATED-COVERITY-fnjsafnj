//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_SIZE][MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void initialize_matrix(Matrix *m, int rows, int cols) {
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = (double)(i * cols + j + 1);
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (!validate_matrix_size(a->rows, b->cols)) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void transpose_matrix(const Matrix *src, Matrix *dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
}

double compute_determinant_2x2(const Matrix *m) {
    if (m->rows != 2 || m->cols != 2) {
        return 0.0;
    }
    return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
}

int main() {
    srand(42);
    
    int rows1 = 3, cols1 = 3;
    int rows2 = 3, cols2 = 3;
    
    if (!validate_matrix_size(rows1, cols1) || !validate_matrix_size(rows2, cols2)) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    Matrix mat1, mat2, result;
    
    initialize_matrix(&mat1, rows1, cols1);
    initialize_matrix(&mat2, rows2, cols2);
    
    printf("Matrix 1:\n");
    print_matrix(&mat1);
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix multiplication:\n");
    if (multiply_matrices(&mat1, &mat2, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot multiply matrices\n");
    }
    
    printf("\nTranspose of Matrix 1:\n");
    Matrix transposed;
    transpose_matrix(&mat1, &transposed);
    print_matrix(&transposed);
    
    printf("\n2x2 determinant test:\n");
    Matrix small_mat;
    initialize_matrix(&small_mat, 2, 2);
    print_matrix(&small_mat);
    printf("Determinant: %.2f\n", compute_determinant_2x2(&small_mat));
    
    return 0;
}