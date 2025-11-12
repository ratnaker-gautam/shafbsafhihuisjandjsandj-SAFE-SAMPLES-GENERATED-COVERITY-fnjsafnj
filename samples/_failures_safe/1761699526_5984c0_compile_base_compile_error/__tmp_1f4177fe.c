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
    printf("Enter scaling factor: ");
    int factor;
    if (scanf("%d", &factor) != 1) {
        fprintf(stderr, "Error: Invalid scaling factor\n");
        while (getchar() != '\n');
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (factor > 0) {
                if (matrix[i][j] > INT_MAX / factor || matrix[i][j] < INT_MIN / factor) {
                    fprintf(stderr, "Error: Integer overflow during scaling\n");
                    return;
                }
            } else if (factor < 0) {
                if (matrix[i][j] < INT_MAX / factor || matrix[i][j] > INT_MIN / factor) {
                    fprintf(stderr, "Error: Integer overflow during scaling\n");
                    return;
                }
            }
            matrix[i][j] *= factor;
        }
    }
}

void add_constant(int rows, int cols, int matrix[rows][cols]) {
    printf("Enter constant to add: ");
    int constant;
    if (scanf("%d", &constant) != 1) {
        fprintf(stderr, "Error: Invalid constant\n");
        while (getchar() != '\n');
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (constant > 0) {
                if (matrix[i][j] > INT_MAX - constant) {
                    fprintf(stderr, "Error: Integer overflow during addition\n");
                    return;
                }
            } else if (constant < 0) {
                if (matrix[i][j] < INT_MIN - constant) {
                    fprintf(stderr, "Error: Integer overflow during addition\n");
                    return;
                }
            }
            matrix[i][j] += constant;
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
}

void apply_operation(int rows, int cols, int matrix[rows][cols], MatrixOp operation) {
    operation(rows, cols, matrix);
}

int main(void) {
    int rows, cols;
    
    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        fprintf(stderr, "Error: Invalid rows input\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter matrix columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        fprintf(stderr, "Error: Invalid columns input\n");
        return EXIT_FAILURE;
    }
    
    validate_matrix_size(rows, cols);
    
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return EXIT_FAILURE;
            }
        }
    }
    
    MatrixOp operations[] = {transpose, scale_matrix, add_constant};
    char* operation_names[] = {"Transpose", "Scale", "Add Constant"};
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        printf("\nCurrent matrix:\n");
        print_matrix(rows, cols, matrix);
        
        printf("\nAvailable operations:\n");
        for (int i = 0; i < num_operations; i++) {
            printf("%d. %s\n", i + 1, operation_names[i]);
        }
        printf("%d. Exit\n", num_operations + 1);
        
        printf("Select operation: ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Error: Invalid choice\n");
            while (getchar() != '\n');
            continue;