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
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return value;
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
            if ((b[i][j] > 0 && a[i][j] > INT_MAX - b[i][j]) ||
                (b[i][j] < 0 && a[i][j] < INT_MIN - b[i][j])) {
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
            if ((b[i][j] < 0 && a[i][j] > INT_MAX + b[i][j]) ||
                (b[i][j] > 0 && a[i][j] < INT_MIN + b[i][j])) {
                result[i][j] = (a[i][j] > 0) ? INT_MAX : INT_MIN;
            } else {
                result[i][j] = a[i][j] - b[i][j];
            }
        }
    }
}

void multiply_matrices(int a_rows, int a_cols, int b_cols, int a[a_rows][a_cols], int b[a_cols][b_cols], int result[a_rows][b_cols]) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a_cols; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    sum = (a[i][k] > 0 && b[k][j] > 0) ? INT_MAX : INT_MIN;
                    break;
                }
                sum += product;
            }
            if (sum > INT_MAX) result[i][j] = INT_MAX;
            else if (sum < INT_MIN) result[i][j] = INT_MIN;
            else result[i][j] = (int)sum;
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
    enum Operation op;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("Available operations:\n");
    printf("0: Add matrices\n");
    printf("1: Subtract matrices\n");
    printf("2: Multiply matrices\n");
    printf("3: Transpose matrix\n");
    
    choice = read_int("Select operation (0-3): ", 0, 3);
    op = (enum Operation)choice;
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT: {
            int rows = read_int("Enter number of rows (1-10): ", 1, 10);
            int cols = read_int("Enter number of columns (1-10): ", 1, 10);
            
            int matrix_a[rows][cols];
            int matrix_b[rows][cols];
            int result[rows][cols];
            
            printf("Enter elements for matrix A (%dx%d):\n", rows, cols);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    matrix_a[i][j] = read_int("", INT_MIN, INT_MAX);
                }
            }
            
            printf