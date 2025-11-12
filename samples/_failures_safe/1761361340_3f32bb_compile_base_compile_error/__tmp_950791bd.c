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
    printf("=========================\n");
    
    rows1 = read_int("Enter number of rows for first matrix (1-10): ", 1, 10);
    cols1 = read_int("Enter number of columns for first matrix (1-10): ", 1, 10);
    
    int matrix1[rows1][cols1];
    printf("Enter elements for first matrix:\n");
    read_matrix(rows1, cols1, matrix1);
    
    printf("First matrix:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nAvailable operations:\n");
    printf("0 - Add\n");
    printf("1 - Subtract\n");
    printf("2 - Multiply\n");
    printf("3 - Transpose\n");
    
    int op_choice = read_int("Select operation (0-3): ", 0, 3);
    enum Operation op = (enum Operation)op_choice;
    
    if (op == OP_ADD || op == OP_SUBTRACT) {
        rows2 = read_int("Enter number of rows for second matrix (1-10): ", 1, 10);
        cols2 = read_int("Enter number of columns for second matrix (1-10): ", 1, 10);
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition/subtraction.\n");
            return 1;
        }
        
        int matrix2[rows2][cols2];
        printf("Enter elements for second matrix:\n");
        read_matrix(rows2, cols2, matrix2);
        
        printf("Second matrix:\n");
        print_matrix(rows2, cols2, matrix2);
        
        int result[rows1][cols1];
        printf("\nResult:\n");
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                if (op == OP_ADD) {
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                } else {
                    result[i][j] = matrix1[i][j] - matrix2[i][j];
                }
                printf("%d ", result[i][j]);
            }
            printf("\n");
        }
    } else if (op == OP_MULTIPLY) {
        rows2 = read_int("Enter number of rows for second matrix (1-10): ", 1, 10);
        cols2 = read_int("Enter number of columns for second matrix (1-10): ", 1, 10);
        
        if (cols1 != rows2) {
            printf("Error: Number of columns in first matrix must equal number of rows in second matrix.\n");
            return 1;
        }
        
        int matrix2[rows2][cols2];
        printf("Enter elements for second matrix:\n");
        read_matrix(rows2, cols2, matrix2);
        
        printf("Second matrix:\n");
        print_matrix(rows2, cols2, matrix2);
        
        int result[rows1][cols2];
        printf("\nResult:\n");
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols2; j++) {
                result[i][j] = 0;
                for (int k = 0; k < cols1; k++) {
                    result[i][j] += matrix1[i][k] * matrix2[k][j];
                }
                printf("%d ", result[i][j]);
            }
            printf("\n");
        }
    } else if (op == OP_TRANSPOSE) {
        printf("\