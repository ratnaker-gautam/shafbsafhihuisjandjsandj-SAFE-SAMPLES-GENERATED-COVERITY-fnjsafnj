//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
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

int validate_matrix_dim(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_int_safe(const char* prompt, int* value) {
    char buffer[32];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char* endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *value = (int)val;
    return 1;
}

void matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void matrix_transpose(const Matrix* src, Matrix* dst) {
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dst->data[j][i] = src->data[i][j];
        }
    }
}

void fill_matrix_sequential(Matrix* mat) {
    int counter = 1;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = counter++;
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Matrix a, b, result, transposed;
    
    printf("Matrix Operations Demo\n");
    
    if (!read_int_safe("Enter rows for matrix A: ", &a.rows)) {
        printf("Invalid input for rows\n");
        return 1;
    }
    if (!read_int_safe("Enter columns for matrix A: ", &a.cols)) {
        printf("Invalid input for columns\n");
        return 1;
    }
    if (!validate_matrix_dim(a.rows, a.cols)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    b.rows = a.cols;
    if (!read_int_safe("Enter columns for matrix B: ", &b.cols)) {
        printf("Invalid input for columns\n");
        return 1;
    }
    if (!validate_matrix_dim(b.rows, b.cols)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    fill_matrix_sequential(&a);
    fill_matrix_sequential(&b);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    if (a.cols != b.rows) {
        printf("Cannot multiply: dimension mismatch\n");
        return 1;
    }
    
    result.rows = a.rows;
    result.cols = b.cols;
    matrix_multiply(&a, &b, &result);
    
    printf("\nMatrix A * B:\n");
    print_matrix(&result);
    
    transposed.rows = result.cols;
    transposed.cols = result.rows;
    matrix_transpose(&result, &transposed);
    
    printf("\nTranspose of result:\n");
    print_matrix(&transposed);
    
    return 0;
}