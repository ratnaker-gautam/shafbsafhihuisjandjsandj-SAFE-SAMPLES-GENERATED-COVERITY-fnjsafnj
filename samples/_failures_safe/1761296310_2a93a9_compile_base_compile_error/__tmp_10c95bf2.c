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
    int value;
    char buffer[100];
    
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
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix *mat) {
    if (mat == NULL) return;
    
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
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
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (product > 0 && sum > LLONG_MAX - product) {
                    free_matrix(result);
                    return NULL;
                }
                if (product < 0 && sum < LLONG_MIN - product) {
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
    
    int rows1 = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    
    Matrix *mat1 = create_matrix(rows1, cols1);
    if (mat1 == NULL) {
        printf("Failed to create matrix 1\n");
        return 1;
    }
    fill_matrix(mat1);
    
    int rows2 = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        free_matrix(mat1);
        return 1;
    }
    
    Matrix *mat2 = create_matrix(rows2, cols2);
    if (mat2 == NULL) {
        printf("Failed to create matrix