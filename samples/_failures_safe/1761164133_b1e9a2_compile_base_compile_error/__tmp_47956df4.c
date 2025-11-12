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
            printf("Invalid input. Please enter a valid integer.\n");
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
    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for first matrix (1-10): ", 1, 10);
    cols1 = read_int("Enter columns for first matrix (1-10): ", 1, 10);
    
    int matrix1[rows1][cols1];
    input_matrix(rows1, cols1, matrix1);
    
    printf("\nFirst matrix:\n");
    print_matrix(rows1, cols1, matrix1);
    
    int op_choice = read_int("\nSelect operation:\n0: Add\n1: Subtract\n2: Multiply\n3: Transpose\nChoice: ", 0, 3);
    
    enum Operation op = (enum Operation)op_choice;
    
    if (op == OP_ADD || op == OP_SUBTRACT) {
        rows2 = read_int("Enter rows for second matrix (1-10): ", 1, 10);
        cols2 = read_int("Enter columns for second matrix (1-10): ", 1, 10);
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition/subtraction.\n");
            return 1;
        }
        
        int matrix2[rows2][cols2];
        input_matrix(rows2, cols2, matrix2);
        
        printf("\nSecond matrix:\n");
        print_matrix(rows2, cols2, matrix2);
        
        printf("\nResult:\n");
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                int result;
                if (op == OP_ADD) {
                    result = matrix1[i][j] + matrix2[i][j];
                } else {
                    result = matrix1[i][j] - matrix2[i][j];
                }
                printf("%6d", result);
            }
            printf("\n");
        }
    } else if (op == OP_MULTIPLY) {
        rows2 = read_int("Enter rows for second matrix (1-10): ", 1, 10);
        cols2 = read_int("Enter columns for second matrix (1-10): ", 1, 10);
        
        if (cols1 != rows2) {
            printf("Error: First matrix columns must equal second matrix rows for multiplication.\n");
            return 1;
        }
        
        int matrix2[rows2][cols2];
        input_matrix(rows2, cols2, matrix2);
        
        printf("\nSecond matrix:\n");
        print_matrix(rows2, cols2, matrix2);
        
        printf("\nResult:\n");
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols2; j++) {
                int sum = 0;
                for (int k = 0; k < cols1; k++) {
                    sum += matrix1[i][k] * matrix2[k][j];
                }
                printf("%6d", sum);
            }
            printf("\n");
        }
    } else if (op == OP_TRANSPOSE) {
        printf("\nTranspose of first matrix:\n");
        for (int j = 0; j < cols1; j++) {
            for (int i = 0; i < rows1; i++) {
                printf("%6d", matrix1[i][j]);
            }
            printf("\n");
        }
    } else {
        printf("Invalid operation selected.\n");
        return 1;
    }
    
    return 0;