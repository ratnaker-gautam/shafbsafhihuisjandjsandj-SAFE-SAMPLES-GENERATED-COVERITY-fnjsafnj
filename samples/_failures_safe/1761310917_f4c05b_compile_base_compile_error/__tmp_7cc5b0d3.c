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
    if (!mat) return;
    
    for (int i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}

int read_matrix_input(Matrix *mat) {
    if (!mat) return 0;
    
    printf("Enter %d x %d matrix elements:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(Matrix *mat) {
    if (!mat) return;
    
    printf("Matrix %dx%d:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

Matrix* multiply_matrices(Matrix *a, Matrix *b) {
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
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        free_matrix(result);
                        return NULL;
                    }
                }
                long long product = (long long)a->data[i][k] * (long long)b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    free_matrix(result);
                    return NULL;
                }
                sum += (int)product;
                if ((sum > 0 && product > 0 && sum < 0) || (sum < 0 && product < 0 && sum > 0)) {
                    free_matrix(result);
                    return NULL;
                }
            }
            result->data[i][j] = sum;
        }
    }
    
    return result;
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter first matrix dimensions (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    Matrix *mat1 = create_matrix(rows1, cols1);
    if (!mat1) {
        printf("Failed to create first matrix\n");
        return 1;
    }
    
    if (!read_matrix_input(mat1)) {
        printf("Failed to read first matrix\n");
        free_matrix(mat1);
        return 1;
    }
    
    printf("Enter second matrix dimensions (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        free_matrix(mat1);
        return 1;
    }
    
    Matrix *mat2 = create_matrix(rows2, cols2);
    if (!mat2) {
        printf("Failed to create second matrix\n");
        free_matrix(mat1);
        return 1;
    }
    
    if (!read_matrix_input(mat2)) {
        printf("Failed to read second matrix\n");
        free_matrix(mat1