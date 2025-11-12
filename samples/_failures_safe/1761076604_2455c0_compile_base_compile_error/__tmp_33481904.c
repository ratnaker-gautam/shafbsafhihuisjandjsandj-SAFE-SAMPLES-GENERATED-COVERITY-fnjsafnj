//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
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

void initialize_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            mat->data[i][j] = read_integer("", INT_MIN, INT_MAX);
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

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_dimensions(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) return 0;
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            long sum = (long)a->data[i][j] + (long)b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) return 0;
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
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
    int choice;
    
    printf("Matrix Operations Program\n");
    
    printf("Enter dimensions for first matrix:\n");
    mat1.rows = read_integer("Rows (1-10): ", 1, MAX_DIM);
    mat1.cols = read_integer("Columns (1-10): ", 1, MAX_DIM);
    
    printf("Enter elements for first matrix:\n");
    initialize_matrix(&mat1);
    
    printf("Enter dimensions for second matrix:\n");
    mat2.rows = read_integer("Rows (1-10): ", 1, MAX_DIM);
    mat2.cols = read_integer("Columns (1-10): ", 1, MAX_DIM);
    
    printf("Enter elements for second matrix:\n");
    initialize_matrix(&mat2);
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    while (1) {
        printf("\nChoose operation:\n");
        printf("1. Matrix Addition\n");
        printf("2. Matrix Multiplication\n");
        printf("3. Matrix Transpose (first matrix)\n");
        printf("4. Exit\n");
        
        choice = read_integer("Enter choice (1-4): ", 1, 4);
        
        if (choice == 4