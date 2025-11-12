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

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter a number between %d and %d\n", min, max);
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols], const char* name) {
    printf("Enter values for %s (%d x %d):\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "[%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

void matrix_add(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void matrix_subtract(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
}

void matrix_multiply(int a_rows, int a_cols, int b_cols, int a[a_rows][a_cols], int b[a_cols][b_cols], int result[a_rows][b_cols]) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            result[i][j] = 0;
            for (int k = 0; k < a_cols; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void matrix_transpose(int rows, int cols, int matrix[rows][cols], int result[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int matrix_determinant_2x2(int matrix[2][2]) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

int main(void) {
    enum Operation op;
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Transpose\n");
    printf("4: Determinant (2x2 only)\n");
    
    op = read_int("Select operation: ", 0, 4);
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            rows1 = read_int("Enter rows for matrices: ", 1, 10);
            cols1 = read_int("Enter columns for matrices: ", 1, 10);
            rows2 = rows1;
            cols2 = cols1;
            break;
            
        case OP_MULTIPLY:
            rows1 = read_int("Enter rows for first matrix: ", 1, 10);
            cols1 = read_int("Enter columns for first matrix: ", 1, 10);
            rows2 = cols1;
            cols2 = read_int("Enter columns for second matrix: ", 1, 10);
            break;
            
        case OP_TRANSPOSE:
            rows1 = read_int("Enter rows for matrix: ", 1, 10);
            cols1 = read_int("Enter columns for matrix: ", 1, 10);
            rows2 = cols1;
            cols2 = rows1;
            break;
            
        case OP_DETERMINANT:
            rows1 = 2;
            cols1 = 2;
            rows2 = 2;
            cols2 = 2;
            break;
    }
    
    int matrix1[rows1][cols1];
    int matrix2[rows2][