//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    return 1;
}

int read_matrix(Matrix *mat) {
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &mat->rows) != 1) return 0;
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
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
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) return 0;
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) return 0;
                } else if (a->data[i][k] != 0 && b->data[k][j] != 0) {
                    if ((a->data[i][k] > 0 && b->data[k][j] < 0 && a->data[i][k] > INT_MAX / b->data[k][j]) ||
                        (a->data[i][k] < 0 && b->data[k][j] > 0 && a->data[i][k] < INT_MIN / b->data[k][j])) {
                        return 0;
                    }
                }
                sum += a->data[i][k] * b->data[k][j];
                if ((sum > 0 && a->data[i][k] * b->data[k][j] > 0 && sum < 0) ||
                    (sum < 0 && a->data[i][k] * b->data[k][j] < 0 && sum > 0)) {
                    return 0;
                }
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix *src, Matrix *dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < dest->rows; i++) {
        for (int j = 0; j < dest->cols; j++) {
            dest->data[i][j] = src->data[j][i];
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    if (!read_matrix(&mat1)) {
        printf("Error: Invalid input for first matrix\n");
        return 1;
    }
    
    printf("Enter second matrix:\n");
    if (!read_matrix(&mat2)) {
        printf("Error: Invalid input for second matrix\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix multiplication:\n");
    if (matrix_multiply(&mat1, &mat2, &result)) {
        printf("Result:\n");
        print_matrix(&result);
    } else {
        printf("Error: Cannot multiply matrices (size mismatch or integer overflow)\n");
    }
    
    printf("\nTranspose of first matrix:\n");
    matrix_transpose(&mat1, &result);
    print_matrix(&result);
    
    printf("\nTranspose of second matrix:\n");
    matrix_transpose(&mat2, &result);
    print_matrix(&result);
    
    return 0;
}