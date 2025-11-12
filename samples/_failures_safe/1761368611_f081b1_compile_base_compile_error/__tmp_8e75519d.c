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
    int value;
    char buffer[100];
    
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
    
    struct Matrix *matrix = malloc(sizeof(struct Matrix));
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

void free_matrix(struct Matrix *matrix) {
    if (matrix == NULL) return;
    
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

void fill_matrix(struct Matrix *matrix) {
    if (matrix == NULL) return;
    
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            matrix->data[i][j] = (i * matrix->cols + j) % 10;
        }
    }
}

void print_matrix(struct Matrix *matrix) {
    if (matrix == NULL) return;
    
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%3d ", matrix->data[i][j]);
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
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
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

int main() {
    printf("Matrix Operations Program\n");
    
    int rows1 = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    if (rows1 == -1) return 1;
    
    int cols1 = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    if (cols1 == -1) return 1;
    
    int rows2 = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    if (rows2 == -1) return 1;
    
    int cols2 = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    if (cols2 == -1) return 1;
    
    if (cols1 != rows2) {
        printf("Matrix multiplication not possible: columns of first must equal rows of second.\n");
        return 1;
    }
    
    struct Matrix *matrix1 = create_matrix(rows1, cols1);
    if (matrix1 == NULL) {
        printf("Failed to create matrix 1.\n");
        return 1;
    }
    
    struct Matrix *matrix2 = create_matrix(rows2, cols2);
    if (matrix2 == NULL) {
        printf("Failed to create matrix