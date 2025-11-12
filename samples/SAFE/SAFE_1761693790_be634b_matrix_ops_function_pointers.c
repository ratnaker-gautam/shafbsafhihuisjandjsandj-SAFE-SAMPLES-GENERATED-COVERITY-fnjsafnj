//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

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

int get_valid_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

void initialize_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j + 1;
        }
    }
}

int main() {
    int rows = get_valid_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    int cols = get_valid_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    int matrix[MAX_SIZE][MAX_SIZE];
    initialize_matrix(rows, cols, matrix);
    
    MatrixOp operations[] = {matrix_add, matrix_multiply, matrix_transpose};
    char* operation_names[] = {"Add 5 to each element", "Multiply each element by 2", "Transpose matrix"};
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("\nOriginal matrix:\n");
    print_matrix(rows, cols, matrix);
    
    for (int i = 0; i < num_operations; i++) {
        printf("\nOperation %d: %s\n", i + 1, operation_names[i]);
        
        int temp_matrix[MAX_SIZE][MAX_SIZE];
        memcpy(temp_matrix, matrix, sizeof(temp_matrix));
        
        operations[i](rows, cols, temp_matrix);
        print_matrix(rows, cols, temp_matrix);
    }
    
    printf("\nFinal matrix (unchanged):\n");
    print_matrix(rows, cols, matrix);
    
    return 0;
}