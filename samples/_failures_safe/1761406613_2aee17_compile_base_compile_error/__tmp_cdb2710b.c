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
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter number of rows for matrix 1 (1-10): ", 1, 10);
    cols1 = read_int("Enter number of columns for matrix 1 (1-10): ", 1, 10);
    
    int matrix1[rows1][cols1];
    printf("Enter elements for matrix 1:\n");
    read_matrix(rows1, cols1, matrix1);
    
    printf("Matrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nAvailable operations:\n");
    printf("0: Add matrices\n");
    printf("1: Subtract matrices\n");
    printf("2: Multiply matrices\n");
    printf("3: Transpose matrix 1\n");
    
    int op_choice = read_int("Select operation (0-3): ", 0, 3);
    enum Operation op = (enum Operation)op_choice;
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT: {
            rows2 = read_int("Enter number of rows for matrix 2: ", 1, 10);
            cols2 = read_int("Enter number of columns for matrix 2: ", 1, 10);
            
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Error: Matrices must have same dimensions for addition/subtraction.\n");
                return 1;
            }
            
            int matrix2[rows2][cols2];
            printf("Enter elements for matrix 2:\n");
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
            break;
        }
        
        case OP_MULTIPLY: {
            rows2 = read_int("Enter number of rows for matrix 2: ", 1, 10);
            cols2 = read_int("Enter number of columns for matrix 2: ", 1, 10);
            
            if (cols1 != rows2) {
                printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
                return 1;
            }
            
            int matrix2[rows2][cols2];
            printf("Enter elements for matrix