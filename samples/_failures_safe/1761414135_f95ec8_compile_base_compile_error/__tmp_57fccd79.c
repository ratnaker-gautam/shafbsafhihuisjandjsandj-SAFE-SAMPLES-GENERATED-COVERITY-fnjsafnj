//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_matrix(Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) return 0;
    if (!validate_matrix_dimensions(m->rows, m->cols)) return 0;
    
    printf("Enter %d elements row by row:\n", m->rows * m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%lf", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_dimensions(a->rows, b->cols)) return 0;
    
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

int transpose_matrix(const Matrix *m, Matrix *result) {
    if (!validate_matrix_dimensions(m->cols, m->rows)) return 0;
    
    result->rows = m->cols;
    result->cols = m->rows;
    
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
    return 1;
}

double compute_determinant(const Matrix *m) {
    if (m->rows != m->cols) return 0.0;
    if (m->rows == 1) return m->data[0][0];
    if (m->rows == 2) {
        return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
    }
    
    double det = 0.0;
    for (int col = 0; col < m->cols; col++) {
        Matrix sub;
        sub.rows = m->rows - 1;
        sub.cols = m->cols - 1;
        
        int sub_i = 0;
        for (int i = 1; i < m->rows; i++) {
            int sub_j = 0;
            for (int j = 0; j < m->cols; j++) {
                if (j == col) continue;
                sub.data[sub_i][sub_j] = m->data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        
        double sub_det = compute_determinant(&sub);
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * m->data[0][col] * sub_det;
    }
    return det;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Maximum matrix dimension: %d\n", MAX_DIM);
    
    printf("\nMatrix A:\n");
    if (!read_matrix(&a)) {
        printf("Error reading matrix A\n");
        return 1;
    }
    
    printf("\nMatrix B:\n");
    if (!read_matrix(&b)) {
        printf("Error reading matrix B\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nMatrix Multiplication (A * B):\n");
    if (multiply_matrices(&a, &b, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot multiply: dimension mismatch\n");
    }
    
    printf("\nTranspose of A:\n");
    if (transpose_matrix(&a, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot transpose: invalid dimensions\n");
    }
    
    printf("\nDeterminant of A: ");
    if (a.rows == a.col