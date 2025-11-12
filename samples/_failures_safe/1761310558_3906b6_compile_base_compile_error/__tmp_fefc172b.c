//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

struct Matrix {
    int rows;
    int cols;
    int **data;
};

int validate_dimension(int dim) {
    return (dim > 0 && dim <= MAX_DIM);
}

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char *endptr;
        long temp = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

struct Matrix* create_matrix(int rows, int cols) {
    if (!validate_dimension(rows) || !validate_dimension(cols)) {
        return NULL;
    }
    
    struct Matrix *mat = malloc(sizeof(struct Matrix));
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

void free_matrix(struct Matrix *mat) {
    if (mat == NULL) return;
    
    for (int i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}

void fill_matrix(struct Matrix *mat) {
    if (mat == NULL) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = (i * mat->cols + j) % 10;
        }
    }
}

void print_matrix(struct Matrix *mat) {
    if (mat == NULL) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%3d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

struct Matrix* multiply_matrices(struct Matrix *a, struct Matrix *b) {
    if (a == NULL || b == NULL || a->cols != b->rows) {
        return NULL;
    }
    
    struct Matrix *result = create_matrix(a->rows, b->cols);
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
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    if (rows1 == -1) return 1;
    
    cols1 = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    if (cols1 == -1) return 1;
    
    rows2 = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    if (rows2 == -1) return 1;
    
    cols2 = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    if (cols2 == -1) return 1;
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        printf("Columns of first matrix must equal rows of second matrix.\n");
        return 1;
    }
    
    struct Matrix *mat1 = create_matrix(rows1, cols1);
    struct Matrix *mat2 = create_matrix(rows2, cols2);
    
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
    
    printf("\nMatrix