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

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int temp[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            temp[j][i] = matrix[i][j];
        }
    }
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            matrix[i][j] = temp[i][j];
        }
    }
}

void scale_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int factor;
    printf("Enter scaling factor: ");
    if (scanf("%d", &factor) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((factor > 0 && matrix[i][j] > INT_MAX / factor) ||
                (factor < 0 && matrix[i][j] < INT_MIN / factor)) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(EXIT_FAILURE);
            }
            matrix[i][j] *= factor;
        }
    }
}

void add_constant(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    int constant;
    printf("Enter constant to add: ");
    if (scanf("%d", &constant) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((constant > 0 && matrix[i][j] > INT_MAX - constant) ||
                (constant < 0 && matrix[i][j] < INT_MIN - constant)) {
                fprintf(stderr, "Error: Integer overflow\n");
                exit(EXIT_FAILURE);
            }
            matrix[i][j] += constant;
        }
    }
}

void apply_operation(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE], MatrixOp op) {
    op(rows, cols, matrix);
}

int main() {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Error: Invalid dimensions\n");
        return EXIT_FAILURE;
    }
    validate_dimensions(rows, cols);
    
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
        add_constant
    };
    
    char* op_names[] = {
        "Print",
        "Transpose",
        "Scale",
        "Add Constant"
    };
    
    int num_ops = sizeof(operations) / sizeof(operations[0]);
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_ops; i++) {
        printf("%d. %s\n", i + 1, op_names[i]);
    }
    
    int choice;
    printf("Select operation (1-%d): ", num_ops);
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Error: Invalid choice\n");
        return EXIT_FAILURE;
    }
    
    if (choice < 1 || choice > num_ops) {
        fprintf(stderr, "Error: Invalid operation\n");
        return EXIT_FAILURE;
    }
    
    apply_operation(rows, cols, matrix, operations[choice - 1]);
    
    if (choice != 1) {
        printf("\nResult:\n");
        if (choice == 2) {
            int temp = rows;
            rows = cols;
            cols = temp;
        }
        print_matrix(rows, cols, matrix);
    }
    
    return EXIT_SUCCESS;
}