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

Matrix* create_matrix(int rows, int cols) {
    if (!validate_dimension(rows) || !validate_dimension(cols)) {
        return NULL;
    }
    
    Matrix *mat = malloc(sizeof(Matrix));
    if (!mat) return NULL;
    
    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(rows * sizeof(int*));
    if (!mat->data) {
        free(mat);
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        mat->data[i] = malloc(cols * sizeof(int));
        if (!mat->data[i]) {
            for (int j = 0; j < i; j++) {
                free(mat->data[j]);
            }
            free(mat->data);
            free(mat);
            return NULL;
        }
    }
    
    return mat;
}

void free_matrix(Matrix *mat) {
    if (!mat) return;
    
    for (int i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}

int read_matrix_input(Matrix *mat) {
    if (!mat) return 0;
    
    printf("Enter %d x %d matrix elements:\n", mat->rows, mat->cols);
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
    if (!mat) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

Matrix* multiply_matrices(const Matrix *a, const Matrix *b) {
    if (!a || !b || a->cols != b->rows) {
        return NULL;
    }
    
    Matrix *result = create_matrix(a->rows, b->cols);
    if (!result) return NULL;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        free_matrix(result);
                        return NULL;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        free_matrix(result);
                        return NULL;
                    }
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    
    return result;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    Matrix *mat1 = create_matrix(rows1, cols1);
    Matrix *mat2 = create_matrix(rows2, cols2);
    
    if (!mat1 || !mat2) {
        printf("Memory allocation failed\n");
        free_matrix(mat1);
        free_matrix(mat2);
        return 1;
    }
    
    printf("First matrix:\n");
    if (!read_matrix_input(mat1)) {
        printf("Failed to read first matrix\n");
        free_matrix(mat1);
        free_matrix(mat2);
        return 1;
    }
    
    printf("Second matrix:\n");
    if (!read_matrix_input(mat2)) {
        printf("Failed to read second matrix\n");
        free_matrix(mat1);
        free_matrix(mat2);
        return 1;
    }
    
    Matrix *result = multiply_matrices(mat1, mat2);
    if (!result) {
        printf("Matrix multiplication failed\n");
        free_matrix(mat1);
        free_matrix(mat2);
        return 1;
    }
    
    printf("Result matrix:\n");
    print_matrix(result);
    
    free_matrix(mat1);
    free_matrix(mat2);
    free_matrix(result);
    
    return 0;
}