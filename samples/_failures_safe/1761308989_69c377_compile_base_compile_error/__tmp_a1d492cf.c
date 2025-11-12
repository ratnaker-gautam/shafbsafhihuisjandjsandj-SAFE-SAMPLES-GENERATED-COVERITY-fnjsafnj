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
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return a + b;
}

int safe_multiply(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return a * b;
}

void matrix_add(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = safe_add(a[i][j], b[i][j]);
        }
    }
}

void matrix_subtract(int rows, int cols, int a[rows][cols], int b[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = safe_add(a[i][j], -b[i][j]);
        }
    }
}

void matrix_multiply(int rows1, int cols1, int a[rows1][cols1], int rows2, int cols2, int b[rows2][cols2], int result[rows1][cols2]) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                int product = safe_multiply(a[i][k], b[k][j]);
                result[i][j] = safe_add(result[i][j], product);
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

int main(void) {
    enum Operation op;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Add\n");
    printf("2. Subtract\n");
    printf("3. Multiply\n");
    printf("4. Transpose\n");
    
    choice = read_int("Select operation (1-4): ", 1, 4);
    op = (enum Operation)(choice - 1);
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT: {
            int rows = read_int("Enter number of rows: ", 1, 10);
            int cols = read_int("Enter number of columns: ", 1, 10);
            
            int matrix1[rows][cols];
            int matrix2[rows][cols];
            int result[rows][cols];
            
            printf("First matrix:\n");
            read_matrix(rows, cols, matrix1);
            printf("Second matrix:\n");
            read_matrix(rows, cols, matrix2);
            
            if (op == OP_ADD) {
                matrix_add(rows, cols, matrix1, matrix2, result);
                printf("Result of addition:\n");
            } else {
                matrix