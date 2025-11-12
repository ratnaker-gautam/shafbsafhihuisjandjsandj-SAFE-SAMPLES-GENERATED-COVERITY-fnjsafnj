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

void add_matrices(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void subtract_matrices(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
}

void multiply_matrices(int rows1, int cols1, int a[rows1][cols1], int rows2, int cols2, int b[rows2][cols2], int result[rows1][cols2]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
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
    
    printf("Matrix Operations:\n");
    printf("1. Add\n");
    printf("2. Subtract\n");
    printf("3. Multiply\n");
    printf("4. Transpose\n");
    
    choice = read_int("Select operation (1-4): ", 1, 4);
    if (choice == -1) {
        return 1;
    }
    op = (enum Operation)(choice - 1);
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT: {
            int rows = read_int("Enter number of rows: ", 1, 10);
            int cols = read_int("Enter number of columns: ", 1, 10);
            int matrix1[rows][cols];
            int matrix2[rows][cols];
            int result[rows][cols];
            
            printf("Enter first matrix:\n");
            read_matrix(rows, cols, matrix1);
            printf("Enter second matrix:\n");
            read_matrix(rows, cols, matrix2);
            
            if (op == OP_ADD) {
                add_matrices(rows, cols, matrix1, matrix2, result);
                printf("Result of addition:\n");
            } else {
                subtract_matrices(rows, cols, matrix1, matrix2, result);
                printf("Result of subtraction:\n");
            }
            print_matrix(rows, cols, result);
            break;
        }
        
        case OP_MULTIPLY: {
            int rows1 = read_int("Enter rows for first matrix: ", 1, 10);
            int cols1 = read_int("Enter columns for first matrix: ", 1, 10);
            int rows2 = read_int("Enter rows for second matrix: ", 1, 10);
            int cols2 = read_int("Enter columns for second matrix: ", 1, 10);
            
            if (cols1 != rows2) {
                printf("Error: Columns of first matrix must equal rows of second matrix.\n");
                return 1;
            }
            
            int matrix1[rows1][cols1];
            int matrix2[rows2][cols2];
            int result