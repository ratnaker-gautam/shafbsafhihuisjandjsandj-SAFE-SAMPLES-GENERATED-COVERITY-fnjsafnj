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
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_ROWS || cols > MAX_COLS) return 0;
    return 1;
}

Matrix* create_matrix(int rows, int cols) {
    if (!validate_matrix_size(rows, cols)) return NULL;
    
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
            for (int j = 0; j < i; j++) free(mat->data[j]);
            free(mat->data);
            free(mat);
            return NULL;
        }
    }
    return mat;
}

void free_matrix(Matrix *mat) {
    if (!mat) return;
    for (int i = 0; i < mat->rows; i++) free(mat->data[i]);
    free(mat->data);
    free(mat);
}

int read_matrix(Matrix *mat) {
    if (!mat) return 0;
    
    printf("Enter %d x %d matrix elements:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    if (!mat) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (!a || !b || !result) return 0;
    if (a->cols != b->rows) return 0;
    if (result->rows != a->rows || result->cols != b->cols) return 0;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    Matrix *mat1 = create_matrix(rows1, cols1);
    if (!mat1) {
        printf("Failed to create matrix 1\n");
        return 1;
    }
    
    if (!read_matrix(mat1)) {
        printf("Failed to read matrix 1\n");
        free_matrix(mat1);
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        free_matrix(mat1);
        return 1;
    }
    
    Matrix *mat2 = create_matrix(rows2, cols2);
    if (!mat2) {
        printf("Failed to create matrix 2\n");
        free_matrix(mat1);
        return 1;
    }
    
    if (!read_matrix(mat2)) {
        printf("Failed to read matrix 2\n");
        free_matrix(mat1);
        free_matrix(mat2);
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        free_matrix(mat1);
        free_matrix(mat2);
        return 1;
    }
    
    Matrix *result = create_matrix(rows1, cols2);
    if (!result) {
        printf("Failed to create result matrix\n");
        free_matrix(mat1);
        free_matrix(mat2);
        return 1;
    }
    
    if (!multiply_matrices(mat1, mat2, result)) {
        printf("Matrix multiplication failed\n");
        free_matrix(mat1);
        free_matrix(mat2);
        free_matrix(result);
        return 1;
    }
    
    printf("Matrix 1:\n");
    print_matrix(mat1);
    printf("Matrix 2:\n");
    print_matrix(mat2);
    printf("Result of multiplication:\n");
    print_matrix(result);
    
    free_matrix(mat1);
    free_matrix(mat2);
    free_matrix(result);
    
    return 0;
}