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

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[256];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
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

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
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
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, 10);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, 10);
    
    int matrix1[rows1][cols1];
    printf("Enter values for matrix 1:\n");
    read_matrix(rows1, cols1, matrix1);
    
    printf("Matrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("Select operation:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose\n");
    
    int op_choice = read_int("Enter operation (0-3): ", 0, 3);
    enum Operation op = (enum Operation)op_choice;
    
    if (op == OP_ADD || op == OP_SUBTRACT) {
        rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, 10);
        cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, 10);
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition/subtraction.\n");
            return 1;
        }
        
        int matrix2[rows2][cols2];
        printf("Enter values for matrix 2:\n");
        read_matrix(rows2, cols2, matrix2);
        
        printf("Matrix 2:\n");
        print_matrix(rows2, cols2, matrix2);
        
        int result[rows1][cols1];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                if (op == OP_ADD) {
                    if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                        (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                        printf("Error: Integer overflow detected.\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                } else {
                    if ((matrix2[i][j] > 0 && matrix1[i][j] < INT_MIN + matrix2[i][j]) ||
                        (matrix2[i][j] < 0 && matrix1[i][j] > INT_MAX + matrix2[i][j])) {
                        printf("Error: Integer overflow detected.\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
            }
        }
        
        printf("Result:\n");
        print_matrix(rows1, cols1, result);
    } else if (op == OP_MULTIPLY) {
        rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, 10);
        cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, 10);
        
        if (cols1 != rows2) {
            printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
            return 1;
        }
        
        int matrix2_mult[rows2][cols2];
        printf("Enter values for matrix 2:\n");
        read_matrix(rows2, cols2, matrix2_mult);
        
        printf("Matrix 2:\n");
        print_matrix(rows2, cols