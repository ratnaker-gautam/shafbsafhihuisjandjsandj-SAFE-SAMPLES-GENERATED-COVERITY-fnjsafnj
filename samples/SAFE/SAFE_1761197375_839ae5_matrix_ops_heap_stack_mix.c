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
    int data[MAX_ROWS][MAX_COLS];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_ROWS || cols > MAX_COLS) return 0;
    return 1;
}

int read_matrix(Matrix *mat) {
    printf("Enter number of rows: ");
    if (scanf("%d", &mat->rows) != 1) return 0;
    printf("Enter number of columns: ");
    if (scanf("%d", &mat->cols) != 1) return 0;
    
    if (!validate_matrix_dimensions(mat->rows, mat->cols)) return 0;
    
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

Matrix* transpose_matrix(const Matrix *mat) {
    Matrix *result = malloc(sizeof(Matrix));
    if (!result) return NULL;
    
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = mat->data[j][i];
        }
    }
    return result;
}

int main() {
    Matrix mat1, mat2;
    
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    if (!read_matrix(&mat1)) {
        printf("Error: Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix:\n");
    if (!read_matrix(&mat2)) {
        printf("Error: Invalid second matrix input\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    Matrix *product = multiply_matrices(&mat1, &mat2);
    if (product) {
        printf("\nMatrix product:\n");
        print_matrix(product);
        free(product);
    } else {
        printf("\nMatrix multiplication failed: dimension mismatch or integer overflow\n");
    }
    
    Matrix *transpose1 = transpose_matrix(&mat1);
    if (transpose1) {
        printf("\nTranspose of first matrix:\n");
        print_matrix(transpose1);
        free(transpose1);
    } else {
        printf("\nTranspose operation failed\n");
    }
    
    Matrix *transpose2 = transpose_matrix(&mat2);
    if (transpose2) {
        printf("\nTranspose of second matrix:\n");
        print_matrix(transpose2);
        free(transpose2);
    } else {
        printf("\nTranspose operation failed\n");
    }
    
    return 0;
}