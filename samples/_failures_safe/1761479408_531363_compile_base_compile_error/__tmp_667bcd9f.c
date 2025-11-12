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
    if (rows <= 0 || rows > MAX_ROWS) return 0;
    if (cols <= 0 || cols > MAX_COLS) return 0;
    return 1;
}

int read_matrix(Matrix *mat) {
    int rows, cols;
    if (scanf("%d %d", &rows, &cols) != 2) return 0;
    if (!validate_matrix_size(rows, cols)) return 0;
    
    mat->rows = rows;
    mat->cols = cols;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
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
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int** allocate_matrix_heap(int rows, int cols) {
    if (!validate_matrix_size(rows, cols)) return NULL;
    
    int **matrix = malloc(rows * sizeof(int*));
    if (!matrix) return NULL;
    
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
        if (!matrix[i]) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix_heap(int **matrix, int rows) {
    if (!matrix) return;
    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

int transpose_matrix_heap(int **src, int **dst, int rows, int cols) {
    if (!src || !dst) return 0;
    if (!validate_matrix_size(rows, cols)) return 0;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[j][i] = src[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix a, b, result_stack;
    
    printf("Enter first matrix (rows cols then elements):\n");
    if (!read_matrix(&a)) {
        printf("Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix (rows cols then elements):\n");
    if (!read_matrix(&b)) {
        printf("Invalid second matrix input\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    if (multiply_matrices(&a, &b, &result_stack)) {
        printf("\nMatrix multiplication result (stack):\n");
        print_matrix(&result_stack);
    } else {
        printf("\nMatrix multiplication failed: invalid dimensions or overflow\n");
    }
    
    int rows = a.rows, cols = a.cols;
    int **heap_matrix = allocate_matrix_heap(rows, cols);
    if (!heap_matrix) {
        printf("Failed to allocate heap matrix\n");
        return 1;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            heap_matrix[i][j] = a.data[i][j];
        }
    }
    
    int **transposed = allocate_matrix_heap(cols, rows);
    if (!transposed) {
        printf("Failed to allocate transposed matrix\n");
        free_matrix_heap(heap_matrix, rows);
        return 1;