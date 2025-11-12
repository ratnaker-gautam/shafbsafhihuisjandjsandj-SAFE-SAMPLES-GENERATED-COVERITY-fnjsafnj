//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: matrix_ops
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

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
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

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
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
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * (long long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    return -1;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                return -1;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix* input, Matrix* result) {
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
    
    choice = read_int("Select operation (1-2): ", 1, 2);
    if (choice == -1) {
        return 1;
    }
    
    if (choice == 1) {
        printf("Matrix A:\n");
        mat1.rows = read_int("Enter rows for matrix A (1-100): ", 1, MAX_DIM);
        mat1.cols = read_int("Enter columns for matrix A (1-100): ", 1, MAX_DIM);
        read_matrix(&mat1);
        
        printf("Matrix B:\n");
        mat2.rows = read_int("Enter rows for matrix B (1-100): ", 1, MAX_DIM);
        mat2.cols = read_int("Enter columns for matrix B (1-100): ", 1, MAX_DIM);
        read_matrix(&mat2);
        
        printf("\nMatrix A:\n");
        print_matrix(&mat1);
        printf("\nMatrix B:\n");
        print_matrix(&mat2);
        
        int mult_result = multiply_matrices(&mat1, &mat2, &result);
        if (mult_result == 0) {
            printf("Error: Cannot multiply - incompatible dimensions.\n");
        } else if (mult_result == -1) {
            printf("Error: Integer overflow during multiplication.\n");
        } else {
            printf("\nResult of A * B:\n");
            print_matrix(&result);
        }
    } else {
        printf("Matrix to transpose:\n");
        mat1.rows = read_int("Enter rows (1-100): ", 1, MAX_DIM);
        mat1.cols = read_int("Enter columns (1-100): ", 1, MAX_DIM);
        read_matrix(&mat1);
        
        printf("\nOriginal matrix:\n");
        print_matrix(&mat1);
        
        transpose_matrix(&mat1, &result);
        printf("\nTransposed matrix:\n");
        print_matrix(&result);
    }
    
    return 0;
}