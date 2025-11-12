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
    int scale;
    printf("Enter scale factor: ");
    if (scanf("%d", &scale) != 1) {
        fprintf(stderr, "Error: Invalid scale factor\n");
        while (getchar() != '\n');
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((scale > 0 && matrix[i][j] > INT_MAX / scale) || 
                (scale < 0 && matrix[i][j] < INT_MIN / scale)) {
                fprintf(stderr, "Error: Integer overflow in scaling\n");
                return;
            }
            matrix[i][j] *= scale;
        }
    }
}

void add_identity(int rows, int cols, int matrix[rows][cols]) {
    if (rows != cols) {
        fprintf(stderr, "Error: Matrix must be square for identity addition\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        if (matrix[i][i] > INT_MAX - 1) {
            fprintf(stderr, "Error: Integer overflow in identity addition\n");
            return;
        }
        matrix[i][i] += 1;
    }
}

void negate_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == INT_MIN) {
                fprintf(stderr, "Error: Integer overflow in negation\n");
                return;
            }
            matrix[i][j] = -matrix[i][j];
        }
    }
}

void apply_operation(MatrixOp operation, int rows, int cols, int matrix[rows][cols]) {
    if (operation == NULL) {
        fprintf(stderr, "Error: Invalid operation\n");
        return;
    }
    operation(rows, cols, matrix);
}

int main(void) {
    int rows, cols;
    
    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        fprintf(stderr, "Error: Invalid input for rows\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter matrix columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        fprintf(stderr, "Error: Invalid input for columns\n");
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
    
    MatrixOp operations[] = {
        print_matrix,
        transpose_matrix,
        scale_matrix,
        add_identity,
        negate_matrix
    };
    
    const char* operation_names[] = {
        "Print",
        "Transpose",
        "Scale",
        "Add Identity",
        "Negate"
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
            return EXIT_FAILURE;
        }
        
        if (choice == num_operations) {
            break;
        }
        
        if (choice < 0 || choice >= num_operations) {