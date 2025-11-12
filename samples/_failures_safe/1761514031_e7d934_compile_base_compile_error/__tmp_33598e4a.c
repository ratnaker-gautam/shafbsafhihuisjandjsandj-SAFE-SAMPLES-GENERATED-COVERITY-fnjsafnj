//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_integer(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer) continue;
        if (*endptr != '\n' && *endptr != '\0') continue;
        if (value < min_val || value > max_val) continue;
        if (value > INT_MAX || value < INT_MIN) continue;
        
        return (int)value;
    }
}

void read_matrix_element(Matrix* mat, int i, int j) {
    char prompt[64];
    snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
    mat->data[i][j] = read_integer(prompt, -1000, 1000);
}

void initialize_matrix(Matrix* mat) {
    printf("Enter number of rows (1-%d): ", MAX_DIM);
    mat->rows = read_integer("", 1, MAX_DIM);
    printf("Enter number of columns (1-%d): ", MAX_DIM);
    mat->cols = read_integer("", 1, MAX_DIM);
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            read_matrix_element(mat, i, j);
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

int can_multiply(const Matrix* a, const Matrix* b) {
    return a->cols == b->rows;
}

int safe_multiply(int a, int b) {
    if (a > 0 && b > 0 && a > INT_MAX / b) return 0;
    if (a > 0 && b < 0 && b < INT_MIN / a) return 0;
    if (a < 0 && b > 0 && a < INT_MIN / b) return 0;
    if (a < 0 && b < 0 && a < INT_MAX / b) return 0;
    return 1;
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (!can_multiply(a, b)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (!safe_multiply(a->data[i][k], b->data[k][j])) return 0;
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

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < dest->rows; i++) {
        for (int j = 0; j < dest->cols; j++) {
            dest->data[i][j] = src->data[j][i];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    printf("First matrix:\n");
    initialize_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    initialize_matrix(&mat2);
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix Multiplication:\n");
    if (multiply_matrices(&mat1, &mat2, &result)) {
        printf("Result:\n");
        print_matrix(&result);
    } else {
        printf("Error: Cannot multiply matrices (dimension mismatch or overflow)\n");
    }
    
    printf("\nTranspose of first matrix:\n");
    transpose_matrix(&mat1, &result);
    print_matrix(&result);
    
    printf("\nTranspose of second matrix:\n");
    transpose_matrix