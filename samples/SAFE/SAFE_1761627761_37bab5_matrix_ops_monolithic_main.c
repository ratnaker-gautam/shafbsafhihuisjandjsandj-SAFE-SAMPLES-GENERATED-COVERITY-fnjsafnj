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
    printf("Matrix %dx%d:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((b->data[i][j] > 0 && a->data[i][j] > INT_MAX - b->data[i][j]) ||
                (b->data[i][j] < 0 && a->data[i][j] < INT_MIN - b->data[i][j])) {
                result->data[i][j] = 0;
            } else {
                result->data[i][j] = a->data[i][j] + b->data[i][j];
            }
        }
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    sum = 0;
                    break;
                }
                sum += product;
            }
            result->data[i][j] = (int)sum;
        }
    }
}

int main(void) {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    read_matrix(&mat1);
    
    mat2.rows = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
    read_matrix(&mat2);
    
    while (1) {
        printf("\nMatrix Operations:\n");
        printf("1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Exit\n");
        
        choice = read_int("Select operation: ", 1, 3);
        
        if (choice == 3) {
            break;
        }
        
        if (choice == 1) {
            if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
                printf("Error: Matrices must have same dimensions for addition.\n");
                continue;
            }
            result.rows = mat1.rows;
            result.cols = mat1.cols;
            add_matrices(&mat1, &mat2, &result);
            printf("Result of addition:\n");
            print_matrix(&result);
        } else if (choice == 2) {
            if (mat1.cols != mat2.rows) {
                printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
                continue;
            }
            result.rows = mat1.rows;
            result.cols = mat2.cols;
            multiply_matrices(&mat1, &mat2, &result);
            printf("Result of multiplication:\n");
            print_matrix(&result);
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}