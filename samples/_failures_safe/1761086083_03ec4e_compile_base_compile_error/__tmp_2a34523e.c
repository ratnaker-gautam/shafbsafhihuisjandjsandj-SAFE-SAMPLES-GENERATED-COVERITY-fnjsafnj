//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*matrix_op_t)(int rows, int cols, int matrix[rows][cols]);

void validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
        exit(EXIT_FAILURE);
    }
}

void validate_matrix_data(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] < -1000 || matrix[i][j] > 1000) {
                fprintf(stderr, "Error: Matrix elements must be between -1000 and 1000\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void transpose_matrix(int rows, int cols, int matrix[rows][cols]) {
    if (rows != cols) {
        fprintf(stderr, "Error: Transpose requires square matrix\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = i + 1; j < cols; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

void scale_matrix(int rows, int cols, int matrix[rows][cols]) {
    int scale_factor;
    printf("Enter scale factor: ");
    if (scanf("%d", &scale_factor) != 1) {
        fprintf(stderr, "Error: Invalid scale factor\n");
        return;
    }
    
    if (scale_factor < -10 || scale_factor > 10) {
        fprintf(stderr, "Error: Scale factor must be between -10 and 10\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long long result = (long long)matrix[i][j] * scale_factor;
            if (result > INT_MAX || result < INT_MIN) {
                fprintf(stderr, "Error: Scaling would cause integer overflow\n");
                return;
            }
            matrix[i][j] = (int)result;
        }
    }
}

void add_constant(int rows, int cols, int matrix[rows][cols]) {
    int constant;
    printf("Enter constant to add: ");
    if (scanf("%d", &constant) != 1) {
        fprintf(stderr, "Error: Invalid constant\n");
        return;
    }
    
    if (constant < -100 || constant > 100) {
        fprintf(stderr, "Error: Constant must be between -100 and 100\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long long result = (long long)matrix[i][j] + constant;
            if (result > INT_MAX || result < INT_MIN) {
                fprintf(stderr, "Error: Addition would cause integer overflow\n");
                return;
            }
            matrix[i][j] = (int)result;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Result matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void apply_operation(int rows, int cols, int matrix[rows][cols], matrix_op_t operation) {
    operation(rows, cols, matrix);
}

int main(void) {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    int choice;
    
    printf("Enter matrix dimensions (rows columns): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Error: Invalid dimensions\n");
        return EXIT_FAILURE;
    }
    
    validate_matrix_size(rows, cols);
    
    printf("Enter matrix elements (%d x %d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return EXIT_FAILURE;
            }
        }
    }
    
    validate_matrix_data(rows, cols, matrix);
    
    printf("\nMatrix operations:\n");
    printf("1. Transpose\n");
    printf("2. Scale\n");
    printf("3. Add constant\n");
    printf("4. Print\n");
    printf("Choose operation (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Error: Invalid choice\n");
        return EXIT_FAILURE;
    }
    
    matrix_op_t operations[] = {transpose_matrix, scale_matrix, add_constant, print_matrix};
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    if (