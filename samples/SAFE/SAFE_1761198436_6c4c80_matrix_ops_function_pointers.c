//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOperation)(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]);

void validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
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
    if (op != NULL) {
        op(rows, cols, matrix);
    }
}

int main(void) {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix dimensions (rows columns, max %dx%d): ", MAX_SIZE, MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Error: Invalid dimensions\n");
        return EXIT_FAILURE;
    }
    
    validate_matrix_size(rows, cols);
    
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
        add_constant,
        NULL
    };
    
    const char* operation_names[] = {
        "Transpose",
        "Scale",
        "Add Constant",
        "Exit"
    };
    
    int choice;
    do {
        printf("\nMatrix Operations:\n");
        for (int i = 0; operations[i] != NULL; i++) {
            printf("%d. %s\n", i + 1, operation_names[i]);
        }
        printf("%d. %s\n", 4, operation_names[3]);
        
        printf("Select operation (1-4): ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Error: Invalid choice\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice >= 1 && choice <= 3) {
            execute_operation(operations[choice - 1], rows, cols, matrix);
            print_matrix(rows, cols, matrix);
        } else if (choice != 4) {
            fprintf(stderr, "Error: Invalid choice\n");
        }
    } while (choice != 4);
    
    return EXIT_SUCCESS;
}