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
    if (mat->data == NULL) return 0;
    
    for (int i = 0; i < rows; i++) {
        mat->data[i] = malloc(cols * sizeof(int));
        if (mat->data[i] == NULL) {
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
    if (mat->data != NULL) {
        for (int i = 0; i < mat->rows; i++) {
            free(mat->data[i]);
        }
        free(mat->data);
        mat->data = NULL;
    }
}

void initialize_matrix(Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = i * mat->cols + j + 1;
        }
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    if (!allocate_matrix(result, a->rows, b->cols)) return 0;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        free_matrix(result);
                        return 0;
                    }
                    int product = a->data[i][k] * b->data[k][j];
                    if (sum > INT_MAX - product) {
                        free_matrix(result);
                        return 0;
                    }
                    sum += product;
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        free_matrix(result);
                        return 0;
                    }
                    int product = a->data[i][k] * b->data[k][j];
                    if (sum < INT_MIN - product) {
                        free_matrix(result);
                        return 0;
                    }
                    sum += product;
                } else {
                    int product = a->data[i][k] * b->data[k][j];
                    if ((product > 0 && sum > INT_MAX - product) ||
                        (product < 0 && sum < INT_MIN - product)) {
                        free_matrix(result);
                        return 0;
                    }
                    sum += product;
                }
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    Matrix a, b, result;
    int stack_buffer[16];
    
    printf("Matrix Multiplication Demo\n");
    printf("Using fixed dimensions 3x4 and 4x2\n");
    
    for (int i = 0; i < 16; i++) {
        stack_buffer[i] = i + 1;
    }
    
    if (!allocate_matrix(&a, 3, 4)) {
        printf("Failed to allocate matrix A\n");
        return 1;
    }
    
    if (!allocate_matrix(&b, 4, 2)) {
        printf("Failed to allocate matrix B\n");
        free_matrix(&a);
        return 1;
    }
    
    initialize_matrix(&a);
    initialize_matrix(&b);
    
    printf("\nMatrix A (3x4):\n");
    print_matrix(&a);
    
    printf("\nMatrix B (4x2):\n");
    print_matrix(&b);
    
    if (multiply_matrices(&a, &b, &result)) {
        printf("\nResult Matrix (3x2):\n");
        print_matrix(&result);
        free_matrix(&result);
    } else {
        printf("Matrix multiplication failed: dimension mismatch or integer overflow\n");
    }
    
    free_matrix(&a);
    free_matrix(&b);
    
    printf("\nStack buffer demonstration:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf