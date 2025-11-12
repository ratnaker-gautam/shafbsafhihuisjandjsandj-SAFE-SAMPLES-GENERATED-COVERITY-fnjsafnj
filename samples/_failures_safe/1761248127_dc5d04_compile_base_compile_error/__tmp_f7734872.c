//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
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

void matrix_multiply(int rows1, int cols1, int a[rows1][cols1], int rows2, int cols2, int b[rows2][cols2], int result[rows1][cols2]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
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
    enum operation op;
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
        case OP_SUBTRACT: {
            rows1 = read_int("Enter rows for both matrices: ", 1, 10);
            cols1 = read_int("Enter columns for both matrices: ", 1, 10);
            
            int matrix_a[rows1][cols1];
            int matrix_b[rows1][cols1];
            int result[rows1][cols1];
            
            printf("Enter first matrix:\n");
            read_matrix(rows1, cols1, matrix_a);
            printf("Enter second matrix:\n");
            read_matrix(rows1, cols1, matrix_b);
            
            if (op == OP_ADD) {
                matrix_add(rows1, cols1, matrix_a, matrix_b, result);
                printf("Result of addition:\n");
            } else {
                matrix_subtract(rows1, cols1, matrix_a, matrix_b, result);
                printf("Result of subtraction:\n");
            }
            print_matrix(rows1, cols1, result);
            break;
        }
            
        case OP_MULTIPLY: {
            rows1 = read_int("Enter rows for first matrix: ", 1, 10);
            cols1 = read_int("Enter columns for first matrix: ", 1, 10);
            rows2 = read_int("Enter rows for second matrix: ", 1, 10);
            cols2 = read_int("Enter