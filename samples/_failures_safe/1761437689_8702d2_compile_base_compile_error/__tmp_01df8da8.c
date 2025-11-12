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
            return INT_MIN;
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

void input_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, 10);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, 10);
    
    int matrix1[rows1][cols1];
    printf("Input matrix 1:\n");
    input_matrix(rows1, cols1, matrix1);
    
    printf("Matrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nAvailable operations:\n");
    printf("0: Add matrices\n");
    printf("1: Subtract matrices\n");
    printf("2: Multiply matrices\n");
    printf("3: Transpose matrix\n");
    
    int op_choice = read_int("Select operation (0-3): ", 0, 3);
    enum Operation operation = (enum Operation)op_choice;
    
    switch (operation) {
        case OP_ADD:
        case OP_SUBTRACT: {
            rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, 10);
            cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, 10);
            
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Error: Matrices must have same dimensions for addition/subtraction.\n");
                return 1;
            }
            
            int matrix2[rows2][cols2];
            printf("Input matrix 2:\n");
            input_matrix(rows2, cols2, matrix2);
            
            printf("Matrix 2:\n");
            print_matrix(rows2, cols2, matrix2);
            
            int result[rows1][cols1];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    if (operation == OP_ADD) {
                        result[i][j] = matrix1[i][j] + matrix2[i][j];
                    } else {
                        result[i][j] = matrix1[i][j] - matrix2[i][j];
                    }
                }
            }
            
            printf("\nResult:\n");
            print_matrix(rows1, cols1, result);
            break;
        }
        
        case OP_MULTIPLY: {
            rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, 10);
            cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, 10);
            
            if (cols1 != rows2) {
                printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
                return 1;
            }
            
            int matrix2[rows2][cols2];
            printf("Input matrix 2:\n");
            input_matrix(rows2, cols2, matrix2);
            
            printf("Matrix 2:\n");
            print_matrix(rows2, cols2, matrix2);
            
            int result[rows1][cols2];
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        result[i][j] += matrix1[i][k] * matrix2[k][j];
                    }
                }
            }
            
            printf("\nResult:\n");
            print_matrix(rows1, cols2, result);
            break;
        }
        
        case OP_TRANSPOSE: