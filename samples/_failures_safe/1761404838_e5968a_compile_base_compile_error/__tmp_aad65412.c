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

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows (1-10): ", 1, MAX_DIM);
    mat->cols = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    printf("Enter %d x %d matrix elements:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("Matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
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

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long sum = (long)a->data[i][j] + (long)b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) {
                return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    
    return 1;
}

void matrix_transpose(const Matrix* a, Matrix* result) {
    result->rows = a->cols;
    result->cols = a->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = a->data[j][i];
        }
    }
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    read_matrix(&a, "A");
    read_matrix(&b, "B");
    
    while (1) {
        printf("\nAvailable operations:\n");
        printf("1. Matrix Addition\n");
        printf("2. Matrix Multiplication\n");
        printf("3. Matrix Transpose (A)\n");
        printf("4. Matrix Transpose (B)\n");
        printf("5. Exit\n");
        
        choice = read_int("Choose operation (1-5): ", 1, 5);
        
        if (choice == 5) {
            break;
        }
        
        switch (choice) {
            case 1:
                if (matrix_add(&a, &b, &result)) {
                    print_matrix(&result, "A + B");
                } else {
                    printf("Matrix addition