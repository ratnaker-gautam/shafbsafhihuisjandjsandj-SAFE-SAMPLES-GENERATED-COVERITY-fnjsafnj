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

int add_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                return 0;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    return 0;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) return 0;
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *mat, Matrix *result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[j][i] = mat->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Add matrices\n");
    printf("2. Multiply matrices\n");
    printf("3. Transpose matrix\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Enter first matrix:\n");
            if (!read_matrix(&mat1)) {
                printf("Invalid first matrix input\n");
                return 1;
            }
            printf("Enter second matrix:\n");
            if (!read_matrix(&mat2)) {
                printf("Invalid second matrix input\n");
                return 1;
            }
            if (!add_matrices(&mat1, &mat2, &result)) {
                printf("Matrix addition failed: size mismatch or integer overflow\n");
                return 1;
            }
            printf("Result of addition:\n");
            print_matrix(&result);
            break;
            
        case 2:
            printf("Enter first matrix:\n");
            if (!read_matrix(&mat1)) {
                printf("Invalid first matrix input\n");
                return 1;
            }
            printf("Enter second matrix:\n");
            if (!read_matrix(&mat2)) {
                printf("Invalid second matrix input\n");
                return 1;
            }
            if (!multiply_matrices(&mat1, &mat