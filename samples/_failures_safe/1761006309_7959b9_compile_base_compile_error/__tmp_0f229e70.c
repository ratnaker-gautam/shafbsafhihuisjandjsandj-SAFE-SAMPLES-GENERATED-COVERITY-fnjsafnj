//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
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

int validate_matrix_dim(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_matrix(Matrix *m) {
    char input[256];
    printf("Enter matrix rows and columns: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d %d", &m->rows, &m->cols) != 2) return 0;
    if (!validate_matrix_dim(m->rows) || !validate_matrix_dim(m->cols)) return 0;
    
    printf("Enter %d x %d matrix elements:\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
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
    if (!validate_matrix_dim(a->rows) || !validate_matrix_dim(b->cols)) return 0;
    
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
    if (!validate_matrix_dim(m->rows) || !validate_matrix_dim(m->cols)) return 0;
    
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
    if (m->rows != m->cols || !validate_matrix_dim(m->rows)) return 0.0;
    
    int n = m->rows;
    double temp[MAX_DIM][MAX_DIM];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[i][j] = m->data[i][j];
        }
    }
    
    double det = 1.0;
    for (int i = 0; i < n; i++) {
        int pivot = i;
        for (int j = i + 1; j < n; j++) {
            if (temp[j][i] > temp[pivot][i]) {
                pivot = j;
            }
        }
        if (pivot != i) {
            for (int j = 0; j < n; j++) {
                double swap = temp[i][j];
                temp[i][j] = temp[pivot][j];
                temp[pivot][j] = swap;
            }
            det = -det;
        }
        if (temp[i][i] == 0.0) return 0.0;
        det *= temp[i][i];
        for (int j = i + 1; j < n; j++) {
            double factor = temp[j][i] / temp[i][i];
            for (int k = i; k < n; k++) {
                temp[j][k] -= factor * temp[i][k];
            }
        }
    }
    return det;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Maximum dimension: %d\n", MAX_DIM);
    
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
        printf("Cannot multiply: dimension mismatch\n