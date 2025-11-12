//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
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
    int data[MAX_ROWS][MAX_COLS];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_ROWS || cols > MAX_COLS) return 0;
    return 1;
}

int read_matrix(Matrix *mat) {
    printf("Enter number of rows: ");
    if (scanf("%d", &mat->rows) != 1) return 0;
    printf("Enter number of columns: ");
    if (scanf("%d", &mat->cols) != 1) return 0;
    
    if (!validate_matrix_size(mat->rows, mat->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

Matrix* multiply_matrices(const Matrix *a, const Matrix *b) {
    if (a->cols != b->rows) return NULL;
    
    Matrix *result = malloc(sizeof(Matrix));
    if (!result) return NULL;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        free(result);
                        return NULL;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        free(result);
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
    Matrix mat1, mat2;
    
    printf("Matrix Multiplication Program\n");
    printf("First matrix:\n");
    if (!read_matrix(&mat1)) {
        printf("Error reading first matrix\n");
        return 1;
    }
    
    printf("Second matrix:\n");
    if (!read_matrix(&mat2)) {
        printf("Error reading second matrix\n");
        return 1;
    }
    
    if (mat1.cols != mat2.rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    Matrix *result = multiply_matrices(&mat1, &mat2);
    if (!result) {
        printf("Error during matrix multiplication\n");
        return 1;
    }
    
    printf("Result matrix:\n");
    print_matrix(result);
    
    free(result);
    return 0;
}