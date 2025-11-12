//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 100
#define MAX_COLS 100

typedef struct {
    int rows;
    int cols;
    int **data;
} Matrix;

int validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || rows > MAX_ROWS) return 0;
    if (cols <= 0 || cols > MAX_COLS) return 0;
    return 1;
}

Matrix* create_matrix(int rows, int cols) {
    if (!validate_matrix_size(rows, cols)) return NULL;
    
    Matrix *matrix = malloc(sizeof(Matrix));
    if (!matrix) return NULL;
    
    matrix->rows = rows;
    matrix->cols = cols;
    
    matrix->data = malloc(rows * sizeof(int*));
    if (!matrix->data) {
        free(matrix);
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        matrix->data[i] = malloc(cols * sizeof(int));
        if (!matrix->data[i]) {
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
    if (!matrix) return;
    
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

int read_matrix_input(Matrix *matrix) {
    if (!matrix) return 0;
    
    printf("Enter %d x %d matrix elements:\n", matrix->rows, matrix->cols);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(const Matrix *matrix) {
    if (!matrix) return;
    
    printf("Matrix (%d x %d):\n", matrix->rows, matrix->cols);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%6d", matrix->data[i][j]);
        }
        printf("\n");
    }
}

Matrix* multiply_matrices(const Matrix *a, const Matrix *b) {
    if (!a || !b) return NULL;
    if (a->cols != b->rows) return NULL;
    
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
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    
    return result;
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    Matrix *matrix1 = create_matrix(rows1, cols1);
    if (!matrix1) {
        printf("Failed to create first matrix\n");
        return 1;
    }
    
    if (!read_matrix_input(matrix1)) {
        printf("Failed to read matrix data\n");
        free_matrix(matrix1);
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        free_matrix(matrix1);
        return 1;
    }
    
    Matrix *matrix2 = create_matrix(rows2, cols2);
    if (!matrix2) {
        printf("Failed to create second matrix\n");
        free_matrix(matrix1);
        return 1;
    }
    
    if (!read_matrix_input(matrix2)) {
        printf("Failed to read matrix data\n");
        free_matrix(matrix1);
        free_matrix(matrix2);
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(matrix1);
    
    printf("\nSecond matrix:\n");
    print_matrix(matrix2);
    
    Matrix *result = multiply_matrices(matrix1, matrix2);
    if (!result) {
        printf("\nMatrix multiplication failed - incompatible dimensions or overflow\n");
    } else {
        printf("\nMultiplication result:\n");
        print_matrix(result);
        free_matrix(result);
    }
    
    free_matrix(matrix1);
    free_matrix(matrix2);
    
    return 0;
}