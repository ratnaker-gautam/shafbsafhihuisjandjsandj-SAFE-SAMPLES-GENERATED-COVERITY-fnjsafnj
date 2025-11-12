//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 100
#define MAX_COLS 100

struct Matrix {
    int rows;
    int cols;
    int data[MAX_ROWS][MAX_COLS];
};

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_ROWS || cols > MAX_COLS) return 0;
    return 1;
}

int read_matrix(struct Matrix *mat) {
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

void print_matrix(const struct Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const struct Matrix *a, const struct Matrix *b, struct Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_dimensions(a->rows, b->cols)) return 0;
    
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

struct Matrix* create_matrix_heap(int rows, int cols) {
    if (!validate_matrix_dimensions(rows, cols)) return NULL;
    struct Matrix *mat = malloc(sizeof(struct Matrix));
    if (mat == NULL) return NULL;
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
    return mat;
}

void free_matrix_heap(struct Matrix *mat) {
    free(mat);
}

int main() {
    struct Matrix stack_mat1, stack_mat2, stack_result;
    struct Matrix *heap_mat1 = NULL;
    struct Matrix *heap_mat2 = NULL;
    struct Matrix *heap_result = NULL;
    
    printf("Matrix Multiplication Program\n");
    printf("First matrix (stack allocated):\n");
    if (!read_matrix(&stack_mat1)) {
        printf("Error reading first matrix\n");
        return 1;
    }
    
    printf("Second matrix (heap allocated):\n");
    heap_mat2 = create_matrix_heap(0, 0);
    if (heap_mat2 == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    if (!read_matrix(heap_mat2)) {
        printf("Error reading second matrix\n");
        free_matrix_heap(heap_mat2);
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&stack_mat1);
    printf("\nSecond matrix:\n");
    print_matrix(heap_mat2);
    
    heap_result = create_matrix_heap(stack_mat1.rows, heap_mat2->cols);
    if (heap_result == NULL) {
        printf("Memory allocation failed\n");
        free_matrix_heap(heap_mat2);
        return 1;
    }
    
    if (!multiply_matrices(&stack_mat1, heap_mat2, heap_result)) {
        printf("Matrix multiplication failed - dimension mismatch or overflow\n");
        free_matrix_heap(heap_mat2);
        free_matrix_heap(heap_result);
        return 1;
    }
    
    printf("\nResult matrix (heap allocated):\n");
    print_matrix(heap_result);
    
    heap_mat1 = create_matrix_heap(2, 2);
    if (heap_mat1 != NULL) {
        heap_mat1->data[0][0] = 1;
        heap_mat1->data[