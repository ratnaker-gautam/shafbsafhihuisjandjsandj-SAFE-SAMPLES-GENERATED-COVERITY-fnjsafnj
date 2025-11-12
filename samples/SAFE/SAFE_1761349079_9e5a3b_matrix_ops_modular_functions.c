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

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

void initialize_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

void input_matrix(Matrix* mat, const char* name) {
    printf("Enter values for matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "  [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        printf("  ");
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    initialize_matrix(result);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * (long long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    return 0;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    
    return 1;
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long long sum = (long long)a->data[i][j] + (long long)b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) {
                result->data[i][j] = 0;
            } else {
                result->data[i][j] = (int)sum;
            }
        }
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    a.rows = read_int("Enter rows for matrix A (1-10): ", 1, MAX_SIZE);
    a.cols = read_int("Enter columns for matrix A (1-10): ", 1, MAX_SIZE);
    input_matrix(&a, "A");
    
    b.rows = read_int("Enter rows for matrix B (1-10): ", 1, MAX_SIZE);
    b.cols = read_int("Enter columns for matrix B (1-10): ", 1, MAX_SIZE);
    input_matrix(&b, "B");
    
    printf("\n");
    print_matrix(&a, "A");
    print_matrix(&b, "B");
    printf("\n");
    
    printf("Matrix Addition:\n");
    if (a.rows == b.rows && a.cols == b.cols) {
        add_matrices(&a, &b, &result);
        print_matrix(&result, "A + B");
    } else {
        printf("Cannot add matrices: dimensions don't match.\n");
    }
    
    printf("\nMatrix Multiplication:\n");
    if (multiply_matrices(&a, &b, &result)) {
        print_matrix(&result, "A * B");
    } else {
        printf("Cannot multiply matrices: invalid dimensions or integer overflow.\n");
    }
    
    return 0;
}