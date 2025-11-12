//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef void (*MatrixOperation)(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]);

void validate_dimensions(int rows, int cols) {
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
}

void transpose(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
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

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    printf("Matrix (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void execute_operation(MatrixOperation op, int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    op(rows, cols, matrix);
}

int main(void) {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix dimensions (rows columns, max %dx%d): ", MAX_SIZE, MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Error: Invalid dimensions input\n");
        return EXIT_FAILURE;
    }
    
    validate_dimensions(rows, cols);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                return EXIT_FAILURE;
            }
        }
    }
    
    MatrixOperation operations[] = {
        transpose,
        scale_matrix,
        add_constant
    };
    
    const char* operation_names[] = {
        "Transpose",
        "Scale",
        "Add Constant"
    };
    
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        printf("\nAvailable operations:\n");
        for (int i = 0; i < num_operations; i++) {
            printf("%d. %s\n", i + 1, operation_names[i]);
        }
        printf("0. Exit\n");
        printf("Choose operation: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Error: Invalid choice\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == 0) {
            break;
        }
        
        if (choice < 1 || choice > num_operations) {
            fprintf(stderr, "Error: Invalid operation choice\n");
            continue;
        }
        
        execute_operation(operations[choice - 1], rows, cols, matrix);
        print_matrix(rows, cols, matrix);
    }
    
    return EXIT_SUCCESS;
}