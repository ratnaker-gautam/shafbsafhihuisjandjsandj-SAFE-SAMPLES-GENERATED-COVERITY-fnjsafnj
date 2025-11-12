//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
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
            mat->data = NULL;
            return 0;
        }
    }
    return 1;
}

void free_matrix(Matrix *mat) {
    if (mat != NULL && mat->data != NULL) {
        for (int i = 0; i < mat->rows; i++) {
            if (mat->data[i] != NULL) {
                free(mat->data[i]);
            }
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
    if (a == NULL || b == NULL || result == NULL) return 0;
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
                }
                if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        free_matrix(result);
                        return 0;
                    }
                }
                int product = a->data[i][k] * b->data[k][j];
                if ((product > 0 && sum > INT_MAX - product) || 
                    (product < 0 && sum < INT_MIN - product)) {
                    free_matrix(result);
                    return 0;
                }
                sum += product;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    if (mat == NULL) return;
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
        return 1;
    }
    
    heap_result->data = NULL;
    heap_result->rows = 0;
    heap_result->cols = 0;
    
    printf("Matrix Multiplication Demo\n");
    printf("Enter dimensions for first matrix (rows cols): ");
    
    int rows1, cols1;
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        free(heap_result);
        return 1;
    }
    
    if (!validate_dimension(rows1) || !validate_dimension(cols1)) {
        printf("Invalid dimensions\n");
        free(heap_result);
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    int rows2, cols2;
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        free(heap_result);
        return 1;
    }
    
    if (!validate_dimension(rows2) || !validate_dimension(cols2)) {
        printf("Invalid dimensions\n");
        free(heap_result);
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        free(heap_result);
        return 1;
    }
    
    if (!allocate_matrix(&stack_matrices[0], rows1, cols1)) {
        printf("Memory allocation failed\n");
        free(heap_result);
        return 1;
    }
    
    if (!allocate_matrix(&stack_matrices