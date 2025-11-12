//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum MatrixOp {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_EXIT
};

int read_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[256];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min_val, max_val);
    }
}

void print_matrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

int** allocate_matrix(int rows, int cols) {
    int** matrix = malloc(rows * sizeof(int*));
    if (matrix == NULL) return NULL;
    
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void input_matrix(int** matrix, int rows, int cols, const char* name) {
    printf("Enter values for matrix %s (%d x %d):\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int("", INT_MIN, INT_MAX);
        }
    }
}

int** matrix_add(int** a, int** b, int rows, int cols) {
    int** result = allocate_matrix(rows, cols);
    if (result == NULL) return NULL;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

int** matrix_subtract(int** a, int** b, int rows, int cols) {
    int** result = allocate_matrix(rows, cols);
    if (result == NULL) return NULL;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

int** matrix_multiply(int** a, int** b, int rows_a, int cols_a, int cols_b) {
    int** result = allocate_matrix(rows_a, cols_b);
    if (result == NULL) return NULL;
    
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols_a; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

int** matrix_transpose(int** matrix, int rows, int cols) {
    int** result = allocate_matrix(cols, rows);
    if (result == NULL) return NULL;
    
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            result[i][j] = matrix[j][i];
        }
    }
    return result;
}

int main() {
    enum MatrixOp operation;
    int rows_a, cols_a, rows_b, cols_b;
    int** matrix_a = NULL;
    int** matrix_b = NULL;
    int** result = NULL;
    
    printf("Matrix Operations Program\n");
    
    while (1) {
        printf("\nAvailable operations:\n");
        printf("0: Add matrices\n");
        printf("1: Subtract matrices\n");
        printf("2: Multiply matrices\n");
        printf("3: Transpose matrix\n");
        printf("4: Exit\n");
        
        operation = read_int("Select operation: ", 0, 4);
        if (operation == OP_EXIT) break;
        
        switch (operation) {
            case OP_ADD:
            case OP_SUBTRACT:
                rows_a = read_int("Enter rows for matrix A: ", 1, 10);
                cols_a = read_int("Enter columns for matrix A: ", 1, 10);
                rows_b = read_int("Enter rows for matrix B: ", 1, 10);
                cols_b = read_int("Enter columns for matrix B: ", 1, 10);
                
                if (rows_a != rows_b || cols_a