//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

enum operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return value;
    }
}

void input_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = get_valid_int(prompt, -1000, 1000);
        }
    }
}

int determinant_2x2(int matrix[2][2]) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

int main(void) {
    enum operation op;
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose\n");
    printf("4: Determinant\n");
    
    op = get_valid_int("Select operation (0-4): ", 0, 4);
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            rows1 = get_valid_int("Enter rows for matrix 1: ", 1, 10);
            cols1 = get_valid_int("Enter columns for matrix 1: ", 1, 10);
            rows2 = get_valid_int("Enter rows for matrix 2: ", 1, 10);
            cols2 = get_valid_int("Enter columns for matrix 2: ", 1, 10);
            
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Error: Matrices must have same dimensions for addition/subtraction.\n");
                return 1;
            }
            
            {
                int mat1[rows1][cols1];
                int mat2[rows1][cols1];
                int result[rows1][cols1];
                
                printf("Enter first matrix:\n");
                input_matrix(rows1, cols1, mat1);
                printf("Enter second matrix:\n");
                input_matrix(rows1, cols1, mat2);
                
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
            }
            break;
            
        case OP_MULTIPLY:
            rows1 = get_valid_int("Enter rows for matrix 1: ", 1, 10);
            cols1 = get_valid_int("Enter columns for matrix 1: ", 1, 10);
            rows2 = get_valid_int("Enter rows for matrix 2: ", 1, 10);
            cols2 = get_valid_int("Enter columns for matrix 2: ", 1, 10);
            
            if (cols1 != rows2) {
                printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
                return 1;
            }
            
            {
                int mat1_mul[rows1][cols1];
                int mat2_mul[rows2][cols2];
                int result_mul[rows1][cols2];
                
                printf("Enter first matrix:\n");
                input_matrix(rows1, cols1, mat1_mul);
                printf("Enter second matrix:\n");
                input_matrix(rows2, cols2, mat2_mul);
                
                for (int i = 0; i < rows1; i++) {
                    for (int j = 0; j < cols2; j++) {
                        result_mul[i][j] = 0;
                        for (int k = 0; k < cols1; k++) {
                            result_mul[i][j] += mat1_mul[i][k] * mat2_mul[k