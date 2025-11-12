//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef void (*MatrixOp)(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]);

void validate_dimensions(int rows, int cols) {
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
}

void read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    printf("Enter %d elements for %dx%d matrix:\n", rows * cols, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid input\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
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

void scale_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int scale;
    printf("Enter scale factor: ");
    if (scanf("%d", &scale) != 1) {
        fprintf(stderr, "Error: Invalid scale factor\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((scale > 0 && matrix[i][j] > INT_MAX / scale) ||
                (scale < 0 && matrix[i][j] < INT_MIN / scale)) {
                fprintf(stderr, "Error: Integer overflow during scaling\n");
                return;
            }
            matrix[i][j] *= scale;
        }
    }
}

void add_constant(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
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

void apply_operation(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE], MatrixOp op) {
    op(rows, cols, matrix);
}

int main(void) {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    
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
    
    validate_dimensions(rows, cols);
    read_matrix(rows, cols, matrix);
    
    MatrixOp operations[] = {
        print_matrix,
        transpose_matrix,
        scale_matrix,
        add_constant
    };
    
    const char* operation_names[] = {
        "Print",
        "Transpose",
        "Scale",
        "Add Constant"
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
            fprintf(stderr, "Error: Invalid operation selection\n");
            continue;
        }
        
        apply_operation(rows, cols, matrix, operations[choice]);
        
        if (choice !=