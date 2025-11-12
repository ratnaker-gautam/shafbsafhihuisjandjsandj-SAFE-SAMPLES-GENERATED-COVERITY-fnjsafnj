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

void print_matrix(Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%4d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(Matrix *a, Matrix *b, Matrix *result) {
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
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int get_positive_int(const char *prompt) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1 && value > 0) {
            return value;
        }
        printf("Invalid input. Please enter a positive integer.\n");
    }
}

int main(void) {
    Matrix mat1, mat2, result;
    
    printf("Matrix Multiplication Program\n");
    
    int rows1 = get_positive_int("Enter rows for matrix 1: ");
    int cols1 = get_positive_int("Enter columns for matrix 1: ");
    int rows2 = get_positive_int("Enter rows for matrix 2: ");
    int cols2 = get_positive_int("Enter columns for matrix 2: ");
    
    if (rows1 <= 0 || cols1 <= 0 || rows2 <= 0 || cols2 <= 0) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    if (!allocate_matrix(&mat1, rows1, cols1)) {
        printf("Failed to allocate matrix 1.\n");
        return 1;
    }
    
    if (!allocate_matrix(&mat2, rows2, cols2)) {
        printf("Failed to allocate matrix 2.\n");
        free_matrix(&mat1);
        return 1;
    }
    
    initialize_matrix(&mat1);
    initialize_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    if (multiply_matrices(&mat1, &mat2, &result)) {
        printf("\nResult of multiplication:\n");
        print_matrix(&result);
        free_matrix(&result);
    } else {
        printf("\nMatrix multiplication failed - dimension mismatch or integer overflow.\n");
    }
    
    free_matrix(&mat1);
    free_matrix(&mat2);
    
    return 0;
}