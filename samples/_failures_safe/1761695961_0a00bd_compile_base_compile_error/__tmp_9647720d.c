//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: matrix_ops
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
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

void init_matrix(Matrix* mat) {
    printf("Initializing matrix %dx%d:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "  Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

void print_matrix(const Matrix* mat) {
    printf("Matrix %dx%d:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        printf("  ");
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Error: Matrices must have same dimensions for addition.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                printf("Error: Integer overflow in addition.\n");
                return;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Error: Incompatible dimensions for multiplication.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    printf("Error: Integer overflow in multiplication.\n");
                    return;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                printf("Error: Integer overflow in multiplication.\n");
                return;
            }
            result->data[i][j] = (int)sum;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    printf("First Matrix:\n");
    mat1.rows = read_int("Enter number of rows (1-10): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter number of columns (1-10): ", 1, MAX_DIM);
    init_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    mat2.rows = read_int("Enter number of rows (1-10): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter number of columns (1-10): ", 1, MAX_DIM);
    init_matrix(&mat2);
    
    printf("\nMatrix A:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix B:\n");
    print_matrix(&mat2);
    
    printf("\nOperations:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    int choice = read_int("Select operation (1-2): ", 1, 2);
    
    switch (choice) {
        case 1:
            add_matrices(&mat1, &mat2, &result);
            if (result.rows > 0 && result.cols > 0) {
                printf("\nResult of Addition:\n");
                print_matrix(&result);
            }
            break;
        case 2:
            multiply_matrices(&mat