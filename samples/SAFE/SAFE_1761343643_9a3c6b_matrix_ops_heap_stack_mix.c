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
    return (dim > 0 && dim <= MAX_DIM);
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
    
    if (!allocate_matrix(result, a->rows, b->cols)) {
        return 0;
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
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
    Matrix stack_matrices[2];
    Matrix *heap_result = malloc(sizeof(Matrix));
    
    if (heap_result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    heap_result->data = NULL;
    
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        fprintf(stderr, "Invalid input\n");
        free(heap_result);
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        fprintf(stderr, "Invalid input\n");
        free(heap_result);
        return 1;
    }
    
    if (!allocate_matrix(&stack_matrices[0], rows1, cols1)) {
        fprintf(stderr, "Failed to allocate first matrix\n");
        free(heap_result);
        return 1;
    }
    
    if (!allocate_matrix(&stack_matrices[1], rows2, cols2)) {
        fprintf(stderr, "Failed to allocate second matrix\n");
        free_matrix(&stack_matrices[0]);
        free(heap_result);
        return 1;
    }
    
    initialize_matrix(&stack_matrices[0]);
    initialize_matrix(&stack_matrices[1]);
    
    printf("\nFirst matrix:\n");
    print_matrix(&stack_matrices[0]);
    
    printf("\nSecond matrix:\n");
    print_matrix(&stack_matrices[1]);
    
    if (multiply_matrices(&stack_matrices[0], &stack_matrices[1], heap_result)) {
        printf("\nMatrix multiplication result:\n");
        print_matrix(heap_result);
    } else {
        printf("\nMatrix multiplication not possible - dimension mismatch\n");
    }
    
    free_matrix(&stack_matrices[0]);
    free_matrix(&stack_matrices[1]);
    free_matrix(heap_result);
    free(heap_result);
    
    return 0;
}