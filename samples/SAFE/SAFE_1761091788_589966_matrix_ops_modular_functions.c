//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
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

int read_int_safe(int *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' && *endptr != '\0') {
        return 0;
    }
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }
    *value = (int)val;
    return 1;
}

int validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        return 0;
    }
    if (rows > MAX_SIZE || cols > MAX_SIZE) {
        return 0;
    }
    return 1;
}

int read_matrix(Matrix *mat) {
    printf("Enter number of rows: ");
    if (!read_int_safe(&mat->rows)) {
        return 0;
    }
    printf("Enter number of columns: ");
    if (!read_int_safe(&mat->cols)) {
        return 0;
    }
    if (!validate_matrix_size(mat->rows, mat->cols)) {
        return 0;
    }
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (!read_int_safe(&mat->data[i][j])) {
                return 0;
            }
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

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    result->rows = a->rows;
    result->cols = b->cols;
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    return 0;
                }
                long long new_sum = (long long)sum + product;
                if (new_sum > INT_MAX || new_sum < INT_MIN) {
                    return 0;
                }
                sum = (int)new_sum;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *mat, Matrix *result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = mat->data[j][i];
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    printf("Enter choice (1 or 2): ");
    
    if (!read_int_safe(&choice) || (choice != 1 && choice != 2)) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice == 1) {
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
        if (!multiply_matrices(&mat1, &mat2, &result)) {
            printf("Matrix multiplication failed - dimension mismatch or overflow\n");
            return 1;
        }
        printf("Result of multiplication:\n");
        print_matrix(&result);
    } else {
        printf("Enter matrix:\n");
        if (!read_matrix(&mat1)) {
            printf("Invalid matrix input\n");
            return 1;
        }
        if (!transpose_matrix(&mat1, &result)) {
            printf("Matrix transpose failed\n");
            return 1;
        }
        printf("Transpose of matrix:\n");
        print_matrix(&result);
    }
    
    return 0;
}