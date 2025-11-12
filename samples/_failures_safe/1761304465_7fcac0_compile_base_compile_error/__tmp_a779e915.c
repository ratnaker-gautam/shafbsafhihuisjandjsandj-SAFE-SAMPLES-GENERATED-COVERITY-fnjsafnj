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

Matrix* create_matrix(int rows, int cols) {
    if (!validate_dimension(rows) || !validate_dimension(cols)) {
        return NULL;
    }
    
    Matrix *matrix = malloc(sizeof(Matrix));
    if (matrix == NULL) {
        return NULL;
    }
    
    matrix->rows = rows;
    matrix->cols = cols;
    
    matrix->data = malloc(rows * sizeof(int*));
    if (matrix->data == NULL) {
        free(matrix);
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        matrix->data[i] = malloc(cols * sizeof(int));
        if (matrix->data[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix->data[j]);
            }
            free(matrix->data);
            free(matrix);
            return NULL;
        }
    }
    
    return matrix;
}

void free_matrix(Matrix *matrix) {
    if (matrix == NULL) return;
    
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

void fill_matrix(Matrix *matrix) {
    if (matrix == NULL) return;
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), 
                    "Element [%d][%d]: ", i + 1, j + 1);
            matrix->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix *matrix) {
    if (matrix == NULL) return;
    
    printf("Matrix (%dx%d):\n", matrix->rows, matrix->cols);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%6d ", matrix->data[i][j]);
        }
        printf("\n");
    }
}

Matrix* multiply_matrices(const Matrix *a, const Matrix *b) {
    if (a == NULL || b == NULL || a->cols != b->rows) {
        return NULL;
    }
    
    Matrix *result = create_matrix(a->rows, b->cols);
    if (result == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    free_matrix(result);
                    return NULL;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                free_matrix(result);
                return NULL;
            }
            result->data[i][j] = (int)sum;
        }
    }
    
    return result;
}

int main() {
    printf("Matrix Multiplication Program\n");
    printf("=============================\n");
    
    int rows1 = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    
    Matrix *matrix1 = create_matrix(rows1, cols1);
    if (matrix1 == NULL) {
        printf("Failed to create matrix 1.\n");
        return 1;
    }
    
    fill_matrix(matrix1);
    
    int rows2 = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        free_matrix(matrix1);
        return 1;
    }
    
    Matrix *matrix2 = create_matrix