//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

enum Operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            exit(EXIT_FAILURE);
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)value;
    }
}

void get_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = get_valid_int(prompt, -1000, 1000);
        }
    }
}

int determinant_2x2(int a, int b, int c, int d) {
    return a * d - b * c;
}

int determinant_3x3(int matrix[3][3]) {
    int det = 0;
    det += matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]);
    det -= matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]);
    det += matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    return det;
}

int main() {
    enum Operation op;
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose\n");
    printf("4: Determinant\n");
    
    op = get_valid_int("Select operation: ", 0, 4);
    
    if (op == OP_ADD || op == OP_SUBTRACT) {
        rows1 = get_valid_int("Enter rows for matrix 1: ", 1, 10);
        cols1 = get_valid_int("Enter columns for matrix 1: ", 1, 10);
        rows2 = get_valid_int("Enter rows for matrix 2: ", 1, 10);
        cols2 = get_valid_int("Enter columns for matrix 2: ", 1, 10);
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition/subtraction.\n");
            return EXIT_FAILURE;
        }
        
        int mat1[rows1][cols1];
        int mat2[rows2][cols2];
        int result[rows1][cols1];
        
        printf("Enter first matrix:\n");
        get_matrix(rows1, cols1, mat1);
        printf("Enter second matrix:\n");
        get_matrix(rows2, cols2, mat2);
        
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                if (op == OP_ADD) {
                    result[i][j] = mat1[i][j] + mat2[i][j];
                } else {
                    result[i][j] = mat1[i][j] - mat2[i][j];
                }
            }
        }
        
        printf("Result:\n");
        print_matrix(rows1, cols1, result);
    } else if (op == OP_MULTIPLY) {
        rows1 = get_valid_int("Enter rows for matrix 1: ", 1, 10);
        cols1 = get_valid_int("Enter columns for matrix 1: ", 1, 10);
        rows2 = get_valid_int("Enter rows for matrix 2: ", 1, 10);
        cols2 = get_valid_int("Enter columns for matrix 2: ", 1, 10);
        
        if (cols1 != rows2) {
            printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
            return EXIT_FAILURE;
        }
        
        int matA[rows1][cols1];
        int matB[rows2][cols2];
        int product[rows1][cols