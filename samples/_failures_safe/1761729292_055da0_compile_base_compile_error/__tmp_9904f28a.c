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

void multiply_matrices(int rows1, int cols1, int a[rows1][cols1], int rows2, int cols2, int b[rows2][cols2], int result[rows1][cols2]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            long long sum = 0;
            for (int k = 0; k < cols1; k++) {
                long long product = (long long)a[i][k] * (long long)b[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    sum = (product > 0) ? INT_MAX : INT_MIN;
                    break;
                }
                sum += product;
            }
            result[i][j] = (sum > INT_MAX) ? INT_MAX : (sum < INT_MIN) ? INT_MIN : (int)sum;
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
    int max_size = 8;
    
    printf("Matrix Operations Program\n");
    printf("Maximum matrix size: %dx%d\n", max_size, max_size);
    
    int rows1 = read_int("Enter rows for first matrix (1-8): ", 1, max_size);
    int cols1 = read_int("Enter columns for first matrix (1-8): ", 1, max_size);
    
    int matrix1[rows1][cols1];
    printf("Enter elements for first matrix:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            matrix1[i][j] = read_int("", INT_MIN, INT_MAX);
        }
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(rows1, cols1, matrix1);
    
    int op_choice = read_int("\nSelect operation (0=Add, 1=Subtract, 2=Multiply, 3=Transpose): ", 0, 3);
    enum Operation operation = (enum Operation)op_choice;
    
    switch (operation) {
        case OP_ADD:
        case OP_SUBTRACT: {
            int rows2 = read_int("Enter rows for second matrix: ", 1, max_size);
            int cols2 = read_int("Enter columns for second matrix: ", 1, max_size);