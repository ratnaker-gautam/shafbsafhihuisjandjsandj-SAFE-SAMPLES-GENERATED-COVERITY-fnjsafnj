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
    printf("%d %d\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d", mat->data[i][j]);
            if (j < mat->cols - 1) printf(" ");
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
                int product = a->data[i][k] * b->data[k][j];
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                sum += product;
            }
            result->data[i][j] = sum;
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

int main(void) {
    Matrix stack_mat1, stack_mat2;
    Matrix *heap_result = NULL;
    
    printf("Enter first matrix (rows cols then elements):\n");
    if (!read_matrix(&stack_mat1)) {
        printf("Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix (rows cols then elements):\n");
    if (!read_matrix(&stack_mat2)) {
        printf("Invalid second matrix input\n");
        return 1;
    }
    
    heap_result = create_matrix_heap(stack_mat1.rows, stack_mat2.cols);
    if (heap_result == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (!multiply_matrices(&stack_mat1, &stack_mat2, heap_result)) {
        printf("Matrix multiplication failed - size mismatch or overflow\n");
        free_matrix_heap(heap_result);
        return 1;
    }
    
    printf("Result matrix:\n");
    print_matrix(heap_result);
    
    free_matrix_heap(heap_result);
    return 0;
}