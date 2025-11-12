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

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Enter value between %d and %d.\n", min_val, max_val);
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

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((a->data[i][k] > 0 && b->data[k][j] > 0 && a->data[i][k] > INT_MAX / b->data[k][j]) ||
                    (a->data[i][k] < 0 && b->data[k][j] < 0 && a->data[i][k] < INT_MAX / b->data[k][j]) ||
                    (a->data[i][k] > 0 && b->data[k][j] < 0 && a->data[i][k] > INT_MIN / b->data[k][j]) ||
                    (a->data[i][k] < 0 && b->data[k][j] > 0 && a->data[i][k] < INT_MIN / b->data[k][j])) {
                    return -1;
                }
                int product = a->data[i][k] * b->data[k][j];
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return -1;
                }
                sum += product;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < dest->rows; i++) {
        for (int j = 0; j < dest->cols; j++) {
            dest->data[i][j] = src->data[j][i];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    read_matrix(&mat1, "A");
    print_matrix(&mat1, "A");
    
    printf("\nChoose operation:\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    
    choice = read_int("Enter choice (1-2): ", 1, 2);
    
    if (choice == 1) {
        read_matrix(&mat2, "B");
        print_matrix(&mat2, "B");
        
        int mult_result = multiply_matrices(&mat1, &mat2, &result);
        if (mult_result == 1) {
            printf("\nMatrix multiplication result:\n");
            print_matrix(&result, "A x B");
        } else if (mult_result == -1) {
            printf("Error: Integer overflow during multiplication.\n");
            return 1;
        } else {
            printf("Error: Incompatible dimensions for multiplication.\n");
            return 1;
        }
    } else {
        transpose_matrix(&mat1, &result