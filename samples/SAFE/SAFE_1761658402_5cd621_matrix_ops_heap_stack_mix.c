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
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_matrix(Matrix *mat) {
    if (scanf("%d %d", &mat->rows, &mat->cols) != 2) {
        return 0;
    }
    if (!validate_dimension(mat->rows) || !validate_dimension(mat->cols)) {
        return 0;
    }
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

Matrix *matrix_multiply(const Matrix *a, const Matrix *b) {
    if (a->cols != b->rows) {
        return NULL;
    }
    
    Matrix *result = malloc(sizeof(Matrix));
    if (result == NULL) {
        return NULL;
    }
    
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
                int product = a->data[i][k] * b->data[k][j];
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    free(result);
                    return NULL;
                }
                sum += product;
            }
            result->data[i][j] = sum;
        }
    }
    
    return result;
}

int main(void) {
    Matrix mat1, mat2;
    
    printf("Enter first matrix (rows cols then elements):\n");
    if (!read_matrix(&mat1)) {
        printf("Invalid first matrix input\n");
        return 1;
    }
    
    printf("Enter second matrix (rows cols then elements):\n");
    if (!read_matrix(&mat2)) {
        printf("Invalid second matrix input\n");
        return 1;
    }
    
    Matrix *result = matrix_multiply(&mat1, &mat2);
    if (result == NULL) {
        printf("Matrix multiplication failed: dimension mismatch or integer overflow\n");
        return 1;
    }
    
    printf("Result matrix:\n");
    print_matrix(result);
    
    free(result);
    return 0;
}