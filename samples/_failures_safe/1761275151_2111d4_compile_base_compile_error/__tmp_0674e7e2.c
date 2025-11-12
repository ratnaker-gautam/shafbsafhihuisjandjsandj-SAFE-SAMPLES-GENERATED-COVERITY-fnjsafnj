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

int read_matrix(Matrix *mat) {
    printf("Enter matrix elements row by row:\n");
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

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    if (!allocate_matrix(result, a->rows, b->cols)) {
        return 0;
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        free_matrix(result);
                        return 0;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MAX / b->data[k][j]) {
                        free_matrix(result);
                        return 0;
                    }
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int main(void) {
    Matrix stack_matrices[2];
    Matrix *heap_result = malloc(sizeof(Matrix));
    if (heap_result == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    heap_result->data = NULL;
    
    for (int i = 0; i < 2; i++) {
        stack_matrices[i].data = NULL;
    }
    
    printf("Enter dimensions for matrix 1 (rows cols): ");
    int rows1, cols1;
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        free(heap_result);
        return 1;
    }
    
    if (!allocate_matrix(&stack_matrices[0], rows1, cols1)) {
        printf("Invalid dimensions or allocation failed\n");
        free(heap_result);
        return 1;
    }
    
    printf("Enter dimensions for matrix 2 (rows cols): ");
    int rows2, cols2;
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        free_matrix(&stack_matrices[0]);
        free(heap_result);
        return 1;
    }
    
    if (!allocate_matrix(&stack_matrices[1], rows2, cols2)) {
        printf("Invalid dimensions or allocation failed\n");
        free_matrix(&stack_matrices[0]);
        free(heap_result);
        return 1;
    }
    
    printf("Matrix 1:\n");
    if (!read_matrix(&stack_matrices[0])) {
        printf("Failed to read matrix 1\n");
        free_matrix(&stack_matrices[0]);
        free_matrix(&stack_matrices[1]);
        free(heap_result);
        return 1;
    }
    
    printf("Matrix 2:\n");
    if (!read_matrix(&stack_matrices[1])) {
        printf("Failed to read matrix 2\n");
        free_matrix(&stack_matrices[0]);
        free_matrix(&stack_matrices[1]);
        free(heap_result