//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]);

void validate_dimensions(int rows, int cols) {
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
}

void transpose(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
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

void scale(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
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

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void execute_operation(MatrixOp op, int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    op(rows, cols, matrix);
}

int main(void) {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Error: Invalid dimensions input\n");
        return EXIT_FAILURE;
    }
    validate_dimensions(rows, cols);
    
    read_matrix(rows, cols, matrix);
    
    printf("\nOriginal matrix:\n");
    print_matrix(rows, cols, matrix);
    
    MatrixOp operations[] = {transpose, scale, add_constant};
    char* names[] = {"Transpose", "Scale", "Add Constant"};
    int num_ops = sizeof(operations) / sizeof(operations[0]);
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_ops; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    int choice;
    printf("\nSelect operation (1-%d): ", num_ops);
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > num_ops) {
        fprintf(stderr, "Error: Invalid operation choice\n");
        return EXIT_FAILURE;
    }
    
    execute_operation(operations[choice - 1], rows, cols, matrix);
    
    if (choice == 1) {
        int temp = rows;
        rows = cols;
        cols = temp;
    }
    
    printf("\nResult matrix:\n");
    print_matrix(rows, cols, matrix);
    
    return EXIT_SUCCESS;
}