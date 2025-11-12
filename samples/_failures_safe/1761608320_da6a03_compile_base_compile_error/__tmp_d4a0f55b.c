//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[rows][cols]);

void validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
        exit(EXIT_FAILURE);
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Enter %d x %d matrix elements:\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid input\n");
                exit(EXIT_FAILURE);
            }
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
    int factor;
    printf("Enter scaling factor: ");
    if (scanf("%d", &factor) != 1) {
        fprintf(stderr, "Error: Invalid scaling factor\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((factor > 0 && matrix[i][j] > INT_MAX / factor) || 
                (factor < 0 && matrix[i][j] < INT_MIN / factor)) {
                fprintf(stderr, "Error: Integer overflow during scaling\n");
                return;
            }
            matrix[i][j] *= factor;
        }
    }
}

void add_constant_matrix(int rows, int cols, int matrix[rows][cols]) {
    int constant;
    printf("Enter constant to add: ");
    if (scanf("%d", &constant) != 1) {
        fprintf(stderr, "Error: Invalid constant\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((constant > 0 && matrix[i][j] > INT_MAX - constant) || 
                (constant < 0 && matrix[i][j] < INT_MIN - constant)) {
                fprintf(stderr, "Error: Integer overflow during addition\n");
                return;
            }
            matrix[i][j] += constant;
        }
    }
}

void negate_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == INT_MIN) {
                fprintf(stderr, "Error: Integer overflow during negation\n");
                return;
            }
            matrix[i][j] = -matrix[i][j];
        }
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
    read_matrix(rows, cols, matrix);
    
    printf("\nOriginal matrix:\n");
    print_matrix(rows, cols, matrix);
    
    MatrixOp operations[] = {
        transpose_matrix,
        scale_matrix,
        add_constant_matrix,
        negate_matrix
    };
    
    const char* operation_names[] = {
        "Transpose",
        "Scale",
        "Add Constant",
        "Negate"
    };
    
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Select operation (1-%d): ", num_operations);
    int choice;
    if (scanf("%