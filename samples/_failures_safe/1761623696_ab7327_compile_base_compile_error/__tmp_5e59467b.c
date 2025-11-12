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
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer) continue;
        if (*endptr != '\n' && *endptr != '\0') continue;
        if (value < min_val || value > max_val) continue;
        if (value == LONG_MIN || value == LONG_MAX) continue;
        
        return (int)value;
    }
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix dimensions:\n");
    mat->rows = read_integer("Rows (1-10): ", 1, MAX_SIZE);
    mat->cols = read_integer("Columns (1-10): ", 1, MAX_SIZE);
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
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

int can_multiply(const Matrix* a, const Matrix* b) {
    return a->cols == b->rows;
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (!can_multiply(a, b)) return 0;
    
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

int can_add(const Matrix* a, const Matrix* b) {
    return a->rows == b->rows && a->cols == b->cols;
}

int add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (!can_add(a, b)) return 0;
    
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

void transpose_matrix(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = input->data[j][i];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("Reading first matrix:\n");
    read_matrix(&mat1);
    
    printf("Reading second matrix:\n");
    read_matrix(&mat2);
    
    while (1) {
        printf("\nAvailable operations:\n");
        printf("1. Matrix Addition\n");
        printf("2. Matrix Multiplication\n");
        printf("3. Transpose First Matrix\n");
        printf("4. Transpose Second Matrix\n");
        printf("5. Exit\n");
        
        choice = read_integer("Choose operation (1-5): ", 1, 5);
        
        if (choice == 5) break;
        
        switch (choice) {
            case 1:
                if (add_matrices(&mat1, &mat2, &result)) {
                    printf("Matrix Addition Result:\n");
                    print_matrix(&result);
                } else {
                    printf("Error: Cannot add matrices - dimension mismatch or overflow\n");
                }