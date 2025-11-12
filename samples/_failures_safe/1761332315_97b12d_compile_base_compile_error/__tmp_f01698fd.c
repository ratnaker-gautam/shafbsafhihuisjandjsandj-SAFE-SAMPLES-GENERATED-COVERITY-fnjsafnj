//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
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

void matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long sum = (long)a->data[i][j] + (long)b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) {
                result->data[i][j] = 0;
            } else {
                result->data[i][j] = (int)sum;
            }
        }
    }
}

int main(void) {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    read_matrix(&mat1, "A");
    read_matrix(&mat2, "B");
    
    printf("\n");
    print_matrix(&mat1, "A");
    printf("\n");
    print_matrix(&mat2, "B");
    printf("\n");
    
    while (1) {
        printf("Choose operation:\n");
        printf("1. Matrix Addition\n");
        printf("2. Matrix Multiplication\n");
        printf("3. Exit\n");
        
        choice = read_int("Enter choice (1-3): ", 1, 3);
        
        if (choice == 3) {
            break;
        }
        
        if (choice == 1) {
            if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
                printf("Error: Matrices must have same dimensions for addition.\n");
                continue;
            }
            matrix_add(&mat1, &mat2, &result);
            printf("Result of A + B:\n");
            print_matrix(&result, "Result");
        } else if (choice == 2) {
            int mult_result = matrix_multiply(&mat1, &mat2, &result);
            if (mult_result == 0) {
                printf("Error: Incompatible dimensions for multiplication.\n");
            } else if (mult_result == -1) {
                printf("Error: Integer overflow during multiplication.\n");
            } else {
                printf("Result of A * B