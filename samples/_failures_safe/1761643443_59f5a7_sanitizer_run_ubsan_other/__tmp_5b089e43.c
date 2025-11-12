//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
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

enum Operation get_operation() {
    printf("Available operations:\n");
    printf("0 - Add matrices\n");
    printf("1 - Subtract matrices\n");
    printf("2 - Multiply matrices\n");
    printf("3 - Transpose matrix\n");
    int choice = read_int("Select operation: ", 0, 3);
    return (enum Operation)choice;
}

int main() {
    enum Operation op = get_operation();
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT: {
            int rows = read_int("Enter number of rows: ", 1, 10);
            int cols = read_int("Enter number of columns: ", 1, 10);
            
            int mat1[rows][cols];
            int mat2[rows][cols];
            int result[rows][cols];
            
            printf("Enter first matrix:\n");
            read_matrix(rows, cols, mat1);
            printf("Enter second matrix:\n");
            read_matrix(rows, cols, mat2);
            
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (op == OP_ADD) {
                        result[i][j] = mat1[i][j] + mat2[i][j];
                    } else {
                        result[i][j] = mat1[i][j] - mat2[i][j];
                    }
                }
            }
            
            printf("Result:\n");
            print_matrix(rows, cols, result);
            break;
        }
        
        case OP_MULTIPLY: {
            int rows1 = read_int("Enter rows for first matrix: ", 1, 10);
            int cols1 = read_int("Enter columns for first matrix: ", 1, 10);
            int rows2 = read_int("Enter rows for second matrix: ", 1, 10);
            int cols2 = read_int("Enter columns for second matrix: ", 1, 10);
            
            if (cols1 != rows2) {
                printf("Error: Cannot multiply matrices. Columns of first must equal rows of second.\n");
                return 1;
            }
            
            int mat1[rows1][cols1];
            int mat2[rows2][cols2];
            int result[rows1][cols2];
            
            printf("Enter first matrix:\n");
            read_matrix(rows1, cols1, mat1);
            printf("Enter second matrix:\n");
            read_matrix(rows2, cols2, mat2);
            
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        result[i][j] += mat1[i][k] * mat2[k][j];
                    }
                }
            }
            
            printf("Result:\n");
            print_matrix(rows1, cols2, result);
            break;
        }
        
        case OP_TRANSPOSE: {
            int rows = read_int("Enter number of rows: ", 1, 10);
            int cols = read_int("Enter number of columns: ", 1, 10);
            
            int matrix[rows][cols];
            int transpose[cols][rows];
            
            printf("Enter matrix:\n");
            read_matrix(rows, cols, matrix);
            
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    transpose[j][i] = matrix[i][j];
                }
            }
            
            printf("Transpose:\n");
            print_matrix(cols, rows, transpose);
            break;
        }
        
        default:
            printf("Invalid operation selected.\n");
            return 1;
    }
    
    return 0;
}