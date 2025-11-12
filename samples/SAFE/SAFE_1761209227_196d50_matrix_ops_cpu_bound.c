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
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_matrix(Matrix *m) {
    char input[256];
    printf("Enter number of rows: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", &m->rows) != 1) return 0;
    
    printf("Enter number of columns: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", &m->cols) != 1) return 0;
    
    if (!validate_dimension(m->rows) || !validate_dimension(m->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) return 0;
            if (sscanf(input, "%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int64_t sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int64_t product = (int64_t)a->data[i][k] * (int64_t)b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) return 0;
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *m, Matrix *result) {
    result->rows = m->cols;
    result->cols = m->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = m->data[j][i];
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("Maximum dimension: %d\n", MAX_DIM);
    
    printf("\nEnter first matrix:\n");
    if (!read_matrix(&a)) {
        printf("Error: Invalid input for first matrix\n");
        return 1;
    }
    
    printf("\nEnter second matrix:\n");
    if (!read_matrix(&b)) {
        printf("Error: Invalid input for second matrix\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&a);
    
    printf("\nSecond matrix:\n");
    print_matrix(&b);
    
    printf("\nMatrix multiplication:\n");
    if (multiply_matrices(&a, &b, &result)) {
        printf("Result:\n");
        print_matrix(&result);
    } else {
        printf("Error: Cannot multiply matrices (dimension mismatch or overflow)\n");
    }
    
    printf("\nTranspose of first matrix:\n");
    if (transpose_matrix(&a, &result)) {
        print_matrix(&result);
    }
    
    printf("\nTranspose of second matrix:\n");
    if (transpose_matrix(&b, &result)) {
        print_matrix(&result);
    }
    
    return 0;
}