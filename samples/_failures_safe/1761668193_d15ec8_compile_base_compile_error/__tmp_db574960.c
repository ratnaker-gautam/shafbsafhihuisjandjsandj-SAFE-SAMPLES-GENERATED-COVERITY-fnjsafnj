//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum Operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_INVALID
};

int read_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[256];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val - 1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min_val, max_val);
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) || 
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                result[i][j] = (a[i][j] > 0) ? INT_MAX : INT_MIN;
            } else {
                result[i][j] = a[i][j] + b[i][j];
            }
        }
    }
}

void subtract_matrices(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((b[i][j] > 0 && a[i][j] < INT_MIN + b[i][j]) || 
                (b[i][j] < 0 && a[i][j] > INT_MAX + b[i][j])) {
                result[i][j] = (b[i][j] > 0) ? INT_MIN : INT_MAX;
            } else {
                result[i][j] = a[i][j] - b[i][j];
            }
        }
    }
}

void multiply_matrices(int rows_a, int cols_a, int cols_b, int a[rows_a][cols_a], int b[cols_a][cols_b], int result[rows_a][cols_b]) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols_a; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX) {
                    result[i][j] = INT_MAX;
                    break;
                } else if (product < INT_MIN) {
                    result[i][j] = INT_MIN;
                    break;
                } else {
                    long long sum = (long long)result[i][j] + product;
                    if (sum > INT_MAX) {
                        result[i][j] = INT_MAX;
                        break;
                    } else if (sum < INT_MIN) {
                        result[i][j] = INT_MIN;
                        break;
                    } else {
                        result[i][j] = (int)sum;
                    }
                }
            }
        }
    }
}

void transpose_matrix(int rows, int cols, int matrix[rows][cols], int result[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int main(void) {
    int max_dim = 8;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    int op_choice = read_int("Choose operation:\n0: Add\n1: Subtract\n2: Multiply\n3: Transpose\nEnter choice: ", 0, 3);
    
    enum Operation op = (enum Operation)op_choice;
    
    int rows_a, cols_a, rows_b, cols_b;
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            rows_a = read_int("Enter rows for matrices (1-8): ", 1, max_dim);
            cols_a = read_int("Enter columns for matrices (1-8): ", 1, max_dim);
            rows_b = rows_a;
            cols_b = cols_a;
            break;
            
        case OP_MULTIPLY:
            rows_a = read_int("Enter rows for matrix A (1-8): ", 1, max_dim);
            cols_a = read_int("Enter columns for matrix A (1-8): ", 1, max_dim);
            rows_b = cols_a;
            cols_b = read_int("Enter columns for matrix B