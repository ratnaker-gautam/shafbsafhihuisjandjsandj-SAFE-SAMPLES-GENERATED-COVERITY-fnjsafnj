//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
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
    printf("%s (%d x %d):\n", name, mat->rows, mat->cols);
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
            int sum = 0;
            int k = 0;
            while (k < a->cols) {
                long long product = (long long)a->data[i][k] * (long long)b->data[k][j];
                if (product > 0 && sum > INT_MAX - product) {
                    return -1;
                }
                if (product < 0 && sum < INT_MIN - product) {
                    return -1;
                }
                sum += (int)product;
                k++;
            }
            result->data[i][j] = sum;
        }
    }
    
    return 1;
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    int i = 0;
    do {
        int j = 0;
        while (j < src->rows) {
            dest->data[i][j] = src->data[j][i];
            j++;
        }
        i++;
    } while (i < src->cols);
}

int matrix_trace(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        return 0;
    }
    
    int trace = 0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i][i];
    }
    return trace;
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
        printf("1. Multiply matrices A * B\n");
        printf("2. Transpose matrix A\n");
        printf("3. Transpose matrix B\n");
        printf("4. Compute trace of A\n");
        printf("5. Compute trace of B\n");
        printf("6. Exit\n");
        
        choice = read_int("Choose operation (1-6): ", 1, 6);
        
        switch (choice) {
            case 1: {
                int mult_result = matrix_multiply(&a, &b, &result);
                if (mult_result == 1) {
                    print_matrix(&result, "A * B");
                } else if (mult_result == -1) {
                    printf("Error: Integer overflow during multiplication.\n");
                } else {
                    printf("Error: Incompatible dimensions for multiplication.\n");
                }
                break;
            }
            case 2:
                matrix_transpose(&a, &result);
                print_matrix(&result, "A transposed");
                break;
            case 3:
                matrix_transpose(&b,