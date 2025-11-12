//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[rows][cols]);

void matrix_add(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] > INT_MAX - 5) {
                matrix[i][j] = 5;
            } else {
                matrix[i][j] += 5;
            }
        }
    }
}

void matrix_multiply(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] > INT_MAX / 2) {
                matrix[i][j] = 2;
            } else {
                matrix[i][j] *= 2;
            }
        }
    }
}

void matrix_transpose(int rows, int cols, int matrix[rows][cols]) {
    if (rows != cols) return;
    
    for (int i = 0; i < rows; i++) {
        for (int j = i + 1; j < cols; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
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

void fill_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = (i * cols + j) % 10;
        }
    }
}

int main() {
    MatrixOp operations[] = {matrix_add, matrix_multiply, matrix_transpose};
    const char* op_names[] = {"Add 5 to each element", "Multiply each element by 2", "Transpose matrix"};
    int num_ops = sizeof(operations) / sizeof(operations[0]);
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows = read_int("Enter number of rows (1-8): ", 1, MAX_SIZE);
    int cols = read_int("Enter number of columns (1-8): ", 1, MAX_SIZE);
    
    int matrix[MAX_SIZE][MAX_SIZE];
    fill_matrix(rows, cols, matrix);
    
    printf("\nOriginal matrix:\n");
    print_matrix(rows, cols, matrix);
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_ops; i++) {
        printf("%d. %s\n", i + 1, op_names[i]);
    }
    
    int choice = read_int("\nSelect operation (1-3): ", 1, num_ops);
    
    MatrixOp selected_op = operations[choice - 1];
    selected_op(rows, cols, matrix);
    
    printf("\nResult matrix:\n");
    print_matrix(rows, cols, matrix);
    
    return 0;
}