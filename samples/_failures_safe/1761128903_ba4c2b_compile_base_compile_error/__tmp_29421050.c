//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int **data;
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int allocate_matrix(Matrix *mat, int rows, int cols) {
    if (!validate_dimension(rows) || !validate_dimension(cols)) {
        return 0;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    
    mat->data = malloc(rows * sizeof(int*));
    if (!mat->data) return 0;
    
    for (int i = 0; i < rows; i++) {
        mat->data[i] = malloc(cols * sizeof(int));
        if (!mat->data[i]) {
            for (int j = 0; j < i; j++) {
                free(mat->data[j]);
            }
            free(mat->data);
            return 0;
        }
    }
    return 1;
}

void free_matrix(Matrix *mat) {
    if (mat && mat->data) {
        for (int i = 0; i < mat->rows; i++) {
            free(mat->data[i]);
        }
        free(mat->data);
    }
}

int read_matrix(Matrix *mat) {
    printf("Enter matrix elements (%d x %d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int add_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    if (!allocate_matrix(result, a->rows, a->cols)) {
        return 0;
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                free_matrix(result);
                return 0;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    if (!allocate_matrix(result, a->rows, b->cols)) {
        return 0;
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    free_matrix(result);
                    return 0;
                }
                long long sum = (long long)result->data[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    free_matrix(result);
                    return 0;
                }
                result->data[i][j] = (int)sum;
            }
        }
    }
    return 1;
}

int main() {
    Matrix stack_matrices[2];
    Matrix *heap_result = malloc(sizeof(Matrix));
    if (!heap_result) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    int rows, cols;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows, &cols) != 2 || !validate_dimension(rows) || !validate_dimension(cols)) {
        printf("Invalid dimensions\n");
        free(heap_result);
        return 1;
    }
    
    if (!allocate_matrix(&stack_matrices[0], rows, cols)) {
        printf("Matrix allocation failed\n");
        free(heap_result);
        return 1;
    }
    
    if (!read_matrix(&stack_matrices[0])) {
        printf("Failed to read matrix\n");
        free_matrix(&stack_matrices[0]);
        free(heap_result);
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows, &cols) != 2 || !validate_dimension(rows) || !validate_dimension(cols)) {
        printf("Invalid dimensions\n");
        free_matrix(&