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

int read_integer(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    
    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return -1;
    if (value < min_val || value > max_val) return -1;
    if (value > INT_MAX || value < INT_MIN) return -1;
    
    return (int)value;
}

int initialize_matrix(Matrix* mat) {
    printf("Enter matrix dimensions:\n");
    mat->rows = read_integer("Rows (1-10): ", 1, MAX_SIZE);
    if (mat->rows == -1) return 0;
    mat->cols = read_integer("Columns (1-10): ", 1, MAX_SIZE);
    if (mat->cols == -1) return 0;
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            int val = read_integer("", INT_MIN, INT_MAX);
            if (val == -1) return 0;
            mat->data[i][j] = val;
        }
    }
    return 1;
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_size(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) return 0;
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix* input, Matrix* result) {
    if (!validate_matrix_size(input->cols, input->rows)) return 0;
    
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = input->data[j][i];
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
    
    choice = read_integer("Select operation (1-2): ", 1, 2);
    if (choice == -1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice == 1) {
        printf("First matrix:\n");
        if (!initialize_matrix(&mat1)) {
            printf("Invalid matrix input\n");
            return 1;
        }
        
        printf("Second matrix:\n");
        if (!initialize_matrix(&mat2)) {
            printf("Invalid matrix input\n");
            return 1;
        }
        
        if (!multiply_matrices(&mat1, &mat2, &result)) {
            printf("Matrix multiplication failed: dimension mismatch or overflow\n");
            return 1;
        }
        
        printf("Result of multiplication:\n");
        print_matrix(&result);
    } else {
        printf("Input matrix:\n");
        if (!initialize_matrix(&mat1)) {
            printf("Invalid matrix input\n");
            return 1;
        }
        
        if (!transpose_matrix(&mat1, &result)) {
            printf("Matrix transpose failed\n");
            return 1;
        }
        
        printf("Transposed matrix:\n");
        print_matrix(&result);
    }
    
    return 0;
}