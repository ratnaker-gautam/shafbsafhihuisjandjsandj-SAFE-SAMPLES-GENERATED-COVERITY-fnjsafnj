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

int validate_matrix_dim(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_matrix(Matrix *m) {
    char input[256];
    
    printf("Enter number of rows: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", &m->rows) != 1) return 0;
    
    printf("Enter number of columns: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", &m->cols) != 1) return 0;
    
    if (!validate_matrix_dim(m->rows, m->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) return 0;
            if (sscanf(input, "%lf", &m->data[i][j]) != 1) return 0;
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
    if (!validate_matrix_dim(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *m, Matrix *result) {
    if (!validate_matrix_dim(m->cols, m->rows)) return 0;
    
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
    Matrix submatrix;
    submatrix.rows = m->rows - 1;
    submatrix.cols = m->cols - 1;
    
    for (int x = 0; x < m->cols; x++) {
        int sub_i = 0;
        for (int i = 1; i < m->rows; i++) {
            int sub_j = 0;
            for (int j = 0; j < m->cols; j++) {
                if (j == x) continue;
                submatrix.data[sub_i][sub_j] = m->data[i][j];
                sub_j++;
            }
            sub_i++;
        }
        double sign = (x % 2 == 0) ? 1.0 : -1.0;
        det += sign * m->data[0][x] * compute_determinant(&submatrix);
    }
    return det;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    printf("Enter first matrix:\n");
    if (!read_matrix(&a)) {
        printf("Error reading first matrix\n");
        return 1;
    }
    
    printf("Enter second matrix:\n");
    if (!read_matrix(&b)) {
        printf("Error reading second matrix\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&a);
    
    printf("\nSecond matrix:\n");
    print_matrix(&b);
    
    printf("\nMatrix multiplication:\n");
    if (multiply_matrices(&a, &b, &result)) {