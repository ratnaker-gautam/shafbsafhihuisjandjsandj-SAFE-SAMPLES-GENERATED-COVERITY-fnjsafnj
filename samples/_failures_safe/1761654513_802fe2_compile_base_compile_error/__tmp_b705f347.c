//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[rows][cols]);

void validate_matrix_params(int rows, int cols) {
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
        exit(EXIT_FAILURE);
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    validate_matrix_params(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int rows, int cols, int matrix[rows][cols]) {
    validate_matrix_params(rows, cols);
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
    validate_matrix_params(rows, cols);
    
    int scale_factor;
    printf("Enter scale factor: ");
    if (scanf("%d", &scale_factor) != 1) {
        fprintf(stderr, "Error: Invalid scale factor\n");
        while (getchar() != '\n');
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
    validate_matrix_params(rows, cols);
    
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

void apply_matrix_operation(int rows, int cols, int matrix[rows][cols], MatrixOp operation) {
    validate_matrix_params(rows, cols);
    if (operation == NULL) {
        fprintf(stderr, "Error: Null operation\n");
        return;
    }
    operation(rows, cols, matrix);
}

int read_matrix(int rows, int cols, int matrix[rows][cols]) {
    validate_matrix_params(rows, cols);
    
    printf("Enter %d x %d matrix elements:\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid input\n");
                while (getchar() != '\n');
                return 0;
            }
        }
    }
    return 1;
}

int main() {
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
    
    validate_matrix_params(rows, cols);
    
    int matrix[MAX_SIZE][MAX_SIZE];
    if (!read_matrix(rows, cols, matrix)) {
        return EXIT_FAILURE;
    }
    
    MatrixOp operations[] = {
        print_matrix,
        transpose_matrix,
        scale_matrix,
        negate_matrix
    };
    
    const char* operation_names[] = {
        "Print Matrix",
        "Transpose Matrix",
        "Scale Matrix",
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
            fprintf(stderr, "Error: