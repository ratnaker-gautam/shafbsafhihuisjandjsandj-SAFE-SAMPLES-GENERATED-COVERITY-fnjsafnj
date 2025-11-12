//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef void (*MatrixOp)(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]);

void validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
        exit(EXIT_FAILURE);
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
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

void scale_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int scale;
    printf("Enter scale factor: ");
    if (scanf("%d", &scale) != 1) {
        fprintf(stderr, "Error: Invalid scale factor\n");
        while (getchar() != '\n');
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

void add_constant(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int constant;
    printf("Enter constant to add: ");
    if (scanf("%d", &constant) != 1) {
        fprintf(stderr, "Error: Invalid constant\n");
        while (getchar() != '\n');
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

void negate_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
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

void execute_operation(MatrixOp op, int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    op(rows, cols, matrix);
}

int main() {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix dimensions (rows columns): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Error: Invalid dimensions\n");
        return EXIT_FAILURE;
    }
    
    validate_matrix_size(rows, cols);
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return EXIT_FAILURE;
            }
        }
    }
    
    MatrixOp operations[] = {
        print_matrix,
        transpose_matrix,
        scale_matrix,
        add_constant,
        negate_matrix
    };
    
    const char* operation_names[] = {
        "Print Matrix",
        "Transpose Matrix",
        "Scale Matrix",
        "Add Constant",
        "Negate Matrix"
    };
    
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        printf("\nAvailable operations:\n");
        for (int i = 0; i < num_operations; i++) {
            printf("%d. %s\n", i, operation_names[i]);
        }
        printf("%d. Exit\n", num_operations);
        
        int choice;
        printf("Select operation: ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Error: Invalid choice\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == num_operations) {
            break;
        }
        
        if (choice < 0 || choice >= num_operations) {
            fprintf