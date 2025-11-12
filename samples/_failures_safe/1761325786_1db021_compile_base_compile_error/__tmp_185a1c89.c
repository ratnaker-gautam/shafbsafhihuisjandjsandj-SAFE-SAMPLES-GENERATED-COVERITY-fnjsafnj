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
            printf("Invalid input. Please enter a valid integer.\n");
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

void matrix_add(int rows, int cols, int A[rows][cols], int B[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((A[i][j] > 0 && B[i][j] > INT_MAX - A[i][j]) ||
                (A[i][j] < 0 && B[i][j] < INT_MIN - A[i][j])) {
                printf("Integer overflow detected at [%d][%d]\n", i, j);
                result[i][j] = 0;
            } else {
                result[i][j] = A[i][j] + B[i][j];
            }
        }
    }
}

void matrix_subtract(int rows, int cols, int A[rows][cols], int B[rows][cols], int result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((B[i][j] > 0 && A[i][j] < INT_MIN + B[i][j]) ||
                (B[i][j] < 0 && A[i][j] > INT_MAX + B[i][j])) {
                printf("Integer overflow detected at [%d][%d]\n", i, j);
                result[i][j] = 0;
            } else {
                result[i][j] = A[i][j] - B[i][j];
            }
        }
    }
}

void matrix_multiply(int rowsA, int colsA, int colsB, int A[rowsA][colsA], int B[colsA][colsB], int result[rowsA][colsB]) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            result[i][j] = 0;
            for (int k = 0; k < colsA; k++) {
                long product = (long)A[i][k] * (long)B[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected at [%d][%d]\n", i, j);
                    result[i][j] = 0;
                    break;
                }
                long sum = (long)result[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow detected at [%d][%d]\n", i, j);
                    result[i][j] = 0;
                    break;
                }
                result[i][j] = (int)sum;
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

int main() {
    enum Operation op;
    int choice;
    
    printf("Matrix Operations Program\n");
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