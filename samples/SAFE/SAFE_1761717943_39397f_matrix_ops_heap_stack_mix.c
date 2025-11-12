//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int **data;
} Matrix;

int validate_dimension(int dim) {
    return (dim > 0 && dim <= MAX_DIM);
}

int allocate_matrix(Matrix *mat, int rows, int cols) {
    if (!validate_dimension(rows) || !validate_dimension(cols)) {
        return 0;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(rows * sizeof(int*));
    if (mat->data == NULL) return 0;
    
    for (int i = 0; i < rows; i++) {
        mat->data[i] = malloc(cols * sizeof(int));
        if (mat->data[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(mat->data[j]);
            }
            free(mat->data);
            return 0;
        }
    }
    return 1;
}

void free_matrix(Matrix *mat) {
    if (mat->data != NULL) {
        for (int i = 0; i < mat->rows; i++) {
            free(mat->data[i]);
        }
        free(mat->data);
    }
}

void initialize_matrix(Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = i * mat->cols + j + 1;
        }
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!allocate_matrix(result, a->rows, b->cols)) return 0;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        free_matrix(result);
                        return 0;
                    }
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    Matrix stack_a, stack_b, heap_result;
    
    stack_a.rows = 3;
    stack_a.cols = 2;
    int a_data[3][2] = {{1, 2}, {3, 4}, {5, 6}};
    stack_a.data = malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; i++) {
        stack_a.data[i] = malloc(2 * sizeof(int));
        memcpy(stack_a.data[i], a_data[i], 2 * sizeof(int));
    }
    
    stack_b.rows = 2;
    stack_b.cols = 3;
    int b_data[2][3] = {{7, 8, 9}, {10, 11, 12}};
    stack_b.data = malloc(2 * sizeof(int*));
    for (int i = 0; i < 2; i++) {
        stack_b.data[i] = malloc(3 * sizeof(int));
        memcpy(stack_b.data[i], b_data[i], 3 * sizeof(int));
    }
    
    printf("Matrix A:\n");
    print_matrix(&stack_a);
    printf("\nMatrix B:\n");
    print_matrix(&stack_b);
    
    heap_result.data = NULL;
    if (multiply_matrices(&stack_a, &stack_b, &heap_result)) {
        printf("\nResult (A * B):\n");
        print_matrix(&heap_result);
        free_matrix(&heap_result);
    } else {
        printf("Matrix multiplication failed.\n");
    }
    
    for (int i = 0; i < 3; i++) {
        free(stack_a.data[i]);
    }
    free(stack_a.data);
    
    for (int i = 0; i < 2; i++) {
        free(stack_b.data[i]);
    }
    free(stack_b.data);
    
    return 0;
}