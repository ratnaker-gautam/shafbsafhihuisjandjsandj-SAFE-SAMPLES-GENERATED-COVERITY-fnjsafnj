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
    return (rows > 0 && rows <= MAX_ROWS && cols > 0 && cols <= MAX_COLS);
}

int read_matrix(Matrix *mat) {
    printf("Enter number of rows and columns: ");
    if (scanf("%d %d", &mat->rows, &mat->cols) != 2) {
        return 0;
    }
    
    if (!validate_matrix_size(mat->rows, mat->cols)) {
        return 0;
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) {
                return 0;
            }
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
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        return 0;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
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

int main() {
    Matrix matrix_a, matrix_b, result;
    
    printf("Matrix A:\n");
    if (!read_matrix(&matrix_a)) {
        printf("Error reading matrix A\n");
        return 1;
    }
    
    printf("Matrix B:\n");
    if (!read_matrix(&matrix_b)) {
        printf("Error reading matrix B\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(&matrix_a);
    
    printf("\nMatrix B:\n");
    print_matrix(&matrix_b);
    
    if (multiply_matrices(&matrix_a, &matrix_b, &result)) {
        printf("\nMatrix multiplication result:\n");
        print_matrix(&result);
    } else {
        printf("\nMatrix multiplication failed: dimension mismatch or integer overflow\n");
        return 1;
    }
    
    int *dynamic_array = malloc(matrix_a.rows * sizeof(int));
    if (dynamic_array == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < matrix_a.rows; i++) {
        dynamic_array[i] = 0;
        for (int j = 0; j < matrix_a.cols; j++) {
            dynamic_array[i] += matrix_a.data[i][j];
        }
    }
    
    printf("\nRow sums of matrix A (using heap):\n");
    for (int i = 0; i < matrix_a.rows; i++) {
        printf("%d ", dynamic_array[i]);
    }
    printf("\n");
    
    free(dynamic_array);
    
    int stack_array[MAX_ROWS];
    for (int i = 0; i < matrix_b.rows; i++) {
        stack_array[i] = 0;
        for (int j = 0; j < matrix_b.cols; j++) {
            stack_array[i] += matrix_b.data[i][j];
        }
    }
    
    printf("Row sums of matrix B (using stack):\n");
    for (int i = 0; i < matrix_b.rows; i++) {
        printf("%d ", stack_array[i]);
    }
    printf("\n");
    
    return 0;
}