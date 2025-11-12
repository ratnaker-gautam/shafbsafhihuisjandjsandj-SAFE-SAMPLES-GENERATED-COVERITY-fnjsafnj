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
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scale_factor > 0 && matrix[i][j] > INT_MAX / scale_factor) {
                fprintf(stderr, "Error: Integer overflow during scaling\n");
                return;
            }
            if (scale_factor < 0 && matrix[i][j] < INT_MIN / scale_factor) {
                fprintf(stderr, "Error: Integer overflow during scaling\n");
                return;
            }
            matrix[i][j] *= scale_factor;
        }
    }
}

void negate_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] != INT_MIN) {
                matrix[i][j] = -matrix[i][j];
            }
        }
    }
}

void apply_operation(int rows, int cols, int matrix[rows][cols], MatrixOp operation) {
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
        fprintf(stderr, "Error: Invalid row input\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter matrix columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        fprintf(stderr, "Error: Invalid column input\n");
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
    
    printf("\nOriginal matrix:\n");
    print_matrix(rows, cols, matrix);
    
    MatrixOp operations[] = {
        print_matrix,
        transpose_matrix,
        scale_matrix,
        negate_matrix
    };
    
    const char* operation_names[] = {
        "Print",
        "Transpose",
        "Scale",
        "Negate"
    };
    
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i, operation_names[i]);
    }
    
    int choice;
    printf("\nSelect operation (0-%d): ", num_operations - 1);
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Error: Invalid operation choice\n");
        return EXIT_FAILURE;
    }
    
    if (choice < 0 || choice >= num_operations) {
        fprintf(stderr, "Error: Operation choice out of range\n");
        return EXIT_FAILURE;
    }
    
    apply_operation(rows, cols, matrix, operations[choice]);
    
    if (choice != 0) {
        printf("\nResult matrix:\n");
        print_matrix(rows, cols, matrix);
    }
    
    return EXIT_SUCCESS;
}