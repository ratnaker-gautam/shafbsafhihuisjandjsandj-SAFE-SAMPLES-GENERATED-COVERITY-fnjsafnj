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

void initialize_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

int input_matrix(Matrix* mat) {
    printf("Enter matrix elements (%d x %d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            int value = read_integer(prompt, -1000, 1000);
            if (value == -1) return 0;
            mat->data[i][j] = value;
        }
    }
    return 1;
}

void print_matrix(const Matrix* mat) {
    printf("Matrix (%d x %d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    initialize_matrix(result);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) return 0;
                if (sum > 0 && product > INT_MAX - sum) return 0;
                if (sum < 0 && product < INT_MIN - sum) return 0;
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) return 0;
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("First Matrix:\n");
    mat1.rows = read_integer("Enter rows (1-10): ", 1, MAX_SIZE);
    mat1.cols = read_integer("Enter columns (1-10): ", 1, MAX_SIZE);
    if (mat1.rows == -1 || mat1.cols == -1) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    if (!input_matrix(&mat1)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    printf("\nSecond Matrix:\n");
    mat2.rows = read_integer("Enter rows (1-10): ", 1, MAX_SIZE);
    mat2.cols = read_integer("Enter columns (1-10): ", 1, MAX_SIZE);
    if (mat2.rows == -1 || mat2.cols == -1) {
        printf("Invalid matrix size\n");
        return 1;
    }
    
    if (!input_matrix(&mat2)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    printf("\nInput Matrices:\n");
    print_matrix(&mat1);
    printf("\n");
    print_matrix(&mat2);
    
    printf("\nMatrix Multiplication:\n");
    if (multiply_matrices(&mat