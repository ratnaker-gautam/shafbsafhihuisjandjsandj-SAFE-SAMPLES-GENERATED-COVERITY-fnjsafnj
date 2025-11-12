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
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min_val, max_val);
    }
}

void input_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = get_valid_int(prompt, -1000, 1000);
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

int matrix_determinant_3x3(int matrix[3][3]) {
    return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
           matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
           matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

int main(void) {
    enum operation op;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("Available operations:\n");
    printf("1. Add\n");
    printf("2. Subtract\n");
    printf("3. Multiply\n");
    printf("4. Transpose\n");
    printf("5. Determinant\n");
    
    choice = get_valid_int("Select operation (1-5): ", 1, 5);
    op = (enum operation)(choice - 1);
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT: {
            int rows = get_valid_int("Enter number of rows: ", 1, 10);
            int cols = get_valid_int("Enter number of columns: ", 1, 10);
            
            int matrix_a[rows][cols];
            int matrix_b[rows][cols];
            int result[rows][cols];
            
            printf("Matrix A:\n");
            input_matrix(rows, cols, matrix_a);
            printf("Matrix B:\n");
            input_matrix(rows, cols, matrix_b);
            
            if (op == OP_ADD) {
                matrix_add(rows, cols, matrix_a, matrix