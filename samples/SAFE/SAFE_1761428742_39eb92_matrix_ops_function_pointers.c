//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[rows][cols]);

void validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
}

void transpose(int rows, int cols, int matrix[rows][cols]) {
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
    int scale;
    printf("Enter scale factor: ");
    if (scanf("%d", &scale) != 1) {
        fprintf(stderr, "Error: Invalid scale factor\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long long result = (long long)matrix[i][j] * scale;
            if (result > INT_MAX || result < INT_MIN) {
                fprintf(stderr, "Error: Integer overflow during scaling\n");
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
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long long result = (long long)matrix[i][j] + constant;
            if (result > INT_MAX || result < INT_MIN) {
                fprintf(stderr, "Error: Integer overflow during addition\n");
                return;
            }
            matrix[i][j] = (int)result;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Matrix %dx%d:\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void input_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Enter %d elements for %dx%d matrix:\n", rows * cols, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void apply_operation(int rows, int cols, int matrix[rows][cols], MatrixOp op) {
    op(rows, cols, matrix);
}

int main(void) {
    int rows, cols;
    
    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        fprintf(stderr, "Error: Invalid row count\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter matrix columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        fprintf(stderr, "Error: Invalid column count\n");
        return EXIT_FAILURE;
    }
    
    validate_matrix_size(rows, cols);
    
    int matrix[MAX_SIZE][MAX_SIZE];
    input_matrix(rows, cols, matrix);
    
    printf("\nOriginal matrix:\n");
    print_matrix(rows, cols, matrix);
    
    MatrixOp operations[] = {transpose, scale_matrix, add_constant};
    char* operation_names[] = {"Transpose", "Scale", "Add Constant"};
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    int choice;
    printf("\nSelect operation (1-%d): ", num_operations);
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > num_operations) {
        fprintf(stderr, "Error: Invalid operation choice\n");
        return EXIT_FAILURE;
    }
    
    apply_operation(rows, cols, matrix, operations[choice - 1]);
    
    printf("\nResult matrix:\n");
    print_matrix(rows, cols, matrix);
    
    return EXIT_SUCCESS;
}