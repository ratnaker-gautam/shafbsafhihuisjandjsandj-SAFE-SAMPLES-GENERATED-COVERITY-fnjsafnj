//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef void (*MatrixOp)(int rows, int cols, int matrix[rows][cols]);

void validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
        exit(EXIT_FAILURE);
    }
}

void validate_matrix_data(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] < -1000 || matrix[i][j] > 1000) {
                fprintf(stderr, "Error: Matrix values must be between -1000 and 1000\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void transpose_matrix(int rows, int cols, int matrix[rows][cols]) {
    if (rows != cols) {
        fprintf(stderr, "Error: Matrix must be square for transpose\n");
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

void negate_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == INT_MIN) {
                fprintf(stderr, "Error: Cannot negate INT_MIN\n");
                return;
            }
            matrix[i][j] = -matrix[i][j];
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Matrix (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void apply_matrix_operation(int rows, int cols, int matrix[rows][cols], MatrixOp operation) {
    operation(rows, cols, matrix);
}

int main(void) {
    int rows, cols;
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        fprintf(stderr, "Error: Invalid row input\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        fprintf(stderr, "Error: Invalid column input\n");
        return EXIT_FAILURE;
    }
    
    validate_matrix_size(rows, cols);
    
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return EXIT_FAILURE;
            }
        }
    }
    
    validate_matrix_data(rows, cols, matrix);
    
    printf("\nOriginal matrix:\n");
    print_matrix(rows, cols, matrix);
    
    MatrixOp operations[] = {transpose_matrix, scale_matrix, negate_matrix};
    const char* operation_names[] = {"Transpose", "Scale", "Negate"};
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    for (int i = 0; i < num_operations; i++) {
        printf("Operation %d: %s\n", i + 1, operation_names[i]);
    }
    
    int choice;
    printf("Select operation (1-%d): ", num_operations);
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Error: Invalid operation choice\n");
        return EXIT