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

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d.\n", min, max);
    }
}

Matrix* create_matrix(int rows, int cols) {
    if (!validate_dimension(rows) || !validate_dimension(cols)) {
        return NULL;
    }
    
    Matrix *mat = malloc(sizeof(Matrix));
    if (mat == NULL) {
        return NULL;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(rows * sizeof(int*));
    if (mat->data == NULL) {
        free(mat);
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        mat->data[i] = malloc(cols * sizeof(int));
        if (mat->data[i] == NULL) {
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
    if (mat == NULL) return;
    
    for (int i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}

void fill_matrix(Matrix *mat) {
    if (mat == NULL) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = (i * mat->cols + j) % 10;
        }
    }
}

void print_matrix(Matrix *mat) {
    if (mat == NULL) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%3d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

Matrix* multiply_matrices(Matrix *a, Matrix *b) {
    if (a == NULL || b == NULL || a->cols != b->rows) {
        return NULL;
    }
    
    Matrix *result = create_matrix(a->rows, b->cols);
    if (result == NULL) {
        return NULL;
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
    
    return result;
}

int main() {
    printf("Matrix Operations Program\n");
    
    int rows1 = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    
    if (rows1 == -1 || cols1 == -1) {
        printf("Input error.\n");
        return 1;
    }
    
    int rows2 = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    
    if (rows2 == -1 || cols2 == -1) {
        printf("Input error.\n");
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return 1;
    }
    
    Matrix *mat1 = create_matrix(rows1, cols1);
    Matrix *mat2 = create_matrix(rows2, cols2);
    
    if (mat1 == NULL || mat2 == NULL) {
        printf("Memory allocation failed.\n");
        if (mat1) free_matrix(mat1);
        if (mat2) free_matrix(mat2);
        return 1;
    }
    
    fill_matrix(mat1);
    fill_matrix(mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(mat2);
    
    Matrix *result = multiply_matrices(mat1, mat2);
    if (result == NULL) {
        printf("Matrix multiplication failed.\n");
    } else {
        printf("\nResult of multiplication:\n");