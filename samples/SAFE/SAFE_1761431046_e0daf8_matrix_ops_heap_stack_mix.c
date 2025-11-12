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
    if (rows <= 0 || rows > MAX_ROWS) return 0;
    if (cols <= 0 || cols > MAX_COLS) return 0;
    return 1;
}

int read_matrix(struct Matrix *mat) {
    if (mat == NULL) return 0;
    
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
    if (mat == NULL) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

struct Matrix* multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    if (a == NULL || b == NULL) return NULL;
    if (a->cols != b->rows) return NULL;
    
    struct Matrix *result = malloc(sizeof(struct Matrix));
    if (result == NULL) return NULL;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((a->data[i][k] > 0 && b->data[k][j] > INT_MAX / a->data[i][k]) ||
                    (a->data[i][k] < 0 && b->data[k][j] < INT_MIN / a->data[i][k])) {
                    free(result);
                    return NULL;
                }
                sum += a->data[i][k] * b->data[k][j];
                if ((a->data[i][k] > 0 && sum < 0) || (a->data[i][k] < 0 && sum > 0)) {
                    free(result);
                    return NULL;
                }
            }
            result->data[i][j] = sum;
        }
    }
    
    return result;
}

int main() {
    struct Matrix mat1, mat2;
    
    printf("Matrix Multiplication Program\n");
    printf("First matrix:\n");
    if (!read_matrix(&mat1)) {
        printf("Error: Invalid first matrix input\n");
        return 1;
    }
    
    printf("Second matrix:\n");
    if (!read_matrix(&mat2)) {
        printf("Error: Invalid second matrix input\n");
        return 1;
    }
    
    if (mat1.cols != mat2.rows) {
        printf("Error: Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    struct Matrix *result = multiply_matrices(&mat1, &mat2);
    if (result == NULL) {
        printf("Error: Matrix multiplication failed\n");
        return 1;
    }
    
    printf("Result matrix:\n");
    print_matrix(result);
    
    free(result);
    return 0;
}