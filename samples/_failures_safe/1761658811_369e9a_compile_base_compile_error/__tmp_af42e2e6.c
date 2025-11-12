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

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_size(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) return 0;
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) return 0;
                }
                sum += a->data[i][k] * b->data[k][j];
                if ((a->data[i][k] > 0 && b->data[k][j] > 0 && sum < 0) ||
                    (a->data[i][k] < 0 && b->data[k][j] < 0 && sum > 0)) {
                    return 0;
                }
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *src, Matrix *dst) {
    if (!validate_matrix_size(src->cols, src->rows)) return 0;
    
    dst->rows = src->cols;
    dst->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
    return 1;
}

Matrix* create_matrix_heap(int rows, int cols) {
    if (!validate_matrix_size(rows, cols)) return NULL;
    Matrix *mat = malloc(sizeof(Matrix));
    if (mat == NULL) return NULL;
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
    return mat;
}

void free_matrix_heap(Matrix *mat) {
    free(mat);
}

int main() {
    Matrix stack_mat1, stack_mat2, stack_result;
    Matrix *heap_mat = NULL;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Matrix 1:\n");
    if (!read_matrix(&stack_mat1)) {
        printf("Error reading matrix 1\n");
        return 1;
    }
    
    printf("Matrix 2:\n");
    if (!read_matrix(&stack_mat2)) {
        printf("Error reading matrix 2\n");
        return 1;
    }
    
    printf("\nMatrix 1:\n");
    print_matrix(&stack_mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&stack_mat2);
    
    printf("\nMatrix Multiplication:\n");
    if (multiply_matrices(&stack_mat1, &stack_mat2, &stack_result)) {
        print_matrix(&stack_result);
    } else {
        printf("Cannot multiply matrices\n");
    }
    
    printf("\nTranspose of Matrix 1:\n");
    heap_mat = create_matrix_heap(stack_mat1.cols, stack_mat1.rows);
    if (heap_mat != NULL) {
        if (transpose_matrix(&stack_mat1, heap_mat)) {
            print_matrix(heap_mat);
        } else {
            printf("Transpose failed\n");
        }
        free_matrix_heap(heap