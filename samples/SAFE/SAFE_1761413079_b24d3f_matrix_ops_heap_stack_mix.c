//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

struct Matrix {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
};

int validate_dimension(int dim) {
    return (dim > 0 && dim <= MAX_DIM);
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

void initialize_matrix(struct Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

void input_matrix(struct Matrix* mat, const char* name) {
    printf("Enter values for matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const struct Matrix* mat, const char* name) {
    printf("Matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const struct Matrix* a, const struct Matrix* b, struct Matrix* result) {
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

int main() {
    struct Matrix mat1, mat2, result;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Exit\n");
    
    operation = read_int("Select operation (1-2): ", 1, 2);
    if (operation == 2) {
        return 0;
    }
    
    printf("Enter dimensions for first matrix:\n");
    mat1.rows = read_int("Rows: ", 1, MAX_DIM);
    mat1.cols = read_int("Columns: ", 1, MAX_DIM);
    
    printf("Enter dimensions for second matrix:\n");
    mat2.rows = read_int("Rows: ", 1, MAX_DIM);
    mat2.cols = read_int("Columns: ", 1, MAX_DIM);
    
    if (operation == 1) {
        if (!validate_dimension(mat1.rows) || !validate_dimension(mat1.cols) ||
            !validate_dimension(mat2.rows) || !validate_dimension(mat2.cols)) {
            printf("Invalid matrix dimensions.\n");
            return 1;
        }
        
        input_matrix(&mat1, "A");
        input_matrix(&mat2, "B");
        
        int multiply_result = multiply_matrices(&mat1, &mat2, &result);
        if (multiply_result == 0) {
            printf("Matrix multiplication not possible: columns of A must equal rows of B.\n");
            return 1;
        } else if (multiply_result == -1) {
            printf("Integer overflow occurred during multiplication.\n");
            return 1;
        }
        
        printf("\n");
        print_matrix(&mat1, "A");
        printf("\n");
        print_matrix(&mat2, "B");
        printf("\n");
        print_matrix(&result, "A x B");
    }
    
    return 0;
}