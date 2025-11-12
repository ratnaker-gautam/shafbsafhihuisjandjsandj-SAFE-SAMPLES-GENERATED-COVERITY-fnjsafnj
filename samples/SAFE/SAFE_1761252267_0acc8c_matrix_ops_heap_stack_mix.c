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
            mat->data[i][j] = (i * mat->cols + j) % 10;
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
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        free_matrix(result);
                        return 0;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
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

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%4d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    Matrix a, b, result;
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
    
    if (!validate_dimension(rows1) || !validate_dimension(cols1) ||
        !validate_dimension(rows2) || !validate_dimension(cols2)) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    if (!allocate_matrix(&a, rows1, cols1)) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (!allocate_matrix(&b, rows2, cols2)) {
        printf("Memory allocation failed\n");
        free_matrix(&a);
        return 1;
    }
    
    initialize_matrix(&a);
    initialize_matrix(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    if (multiply_matrices(&a, &b, &result)) {
        printf("\nResult of A * B:\n");
        print_matrix(&result);
        free_matrix(&result);
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    free_matrix(&a);
    free_matrix(&b);
    
    return 0;
}