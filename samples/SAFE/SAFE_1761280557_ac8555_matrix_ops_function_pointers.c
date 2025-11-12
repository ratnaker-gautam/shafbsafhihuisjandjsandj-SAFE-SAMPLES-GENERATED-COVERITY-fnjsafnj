//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]);

void validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
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
        fprintf(stderr, "Error: Invalid scaling factor\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((factor > 0 && matrix[i][j] > INT_MAX / factor) ||
                (factor < 0 && matrix[i][j] < INT_MIN / factor)) {
                fprintf(stderr, "Error: Integer overflow during scaling\n");
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
        fprintf(stderr, "Error: Invalid constant\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((constant > 0 && matrix[i][j] > INT_MAX - constant) ||
                (constant < 0 && matrix[i][j] < INT_MIN - constant)) {
                fprintf(stderr, "Error: Integer overflow during addition\n");
                exit(EXIT_FAILURE);
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

void initialize_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    printf("Enter %d matrix elements row by row:\n", rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error: Invalid matrix element\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int main(void) {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    MatrixOp operations[] = {transpose, scale, add_constant};
    char *op_names[] = {"Transpose", "Scale", "Add Constant"};
    int num_ops = sizeof(operations) / sizeof(operations[0]);

    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1) {
        fprintf(stderr, "Error: Invalid row count\n");
        return EXIT_FAILURE;
    }
    printf("Enter matrix columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1) {
        fprintf(stderr, "Error: Invalid column count\n");
        return EXIT_FAILURE;
    }

    validate_matrix_size(rows, cols);
    initialize_matrix(rows, cols, matrix);
    print_matrix(rows, cols, matrix);

    int choice;
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_ops; i++) {
        printf("%d. %s\n", i + 1, op_names[i]);
    }
    printf("Select operation (1-%d): ", num_ops);
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > num_ops) {
        fprintf(stderr, "Error: Invalid operation choice\n");
        return EXIT_FAILURE;
    }

    MatrixOp selected_op = operations[choice - 1];
    if (choice == 1) {
        selected_op(rows, cols, matrix);
        int temp = rows;
        rows = cols;
        cols = temp;
    } else {
        selected_op(rows, cols, matrix);
    }

    printf("\nResult:\n");
    print_matrix(rows, cols, matrix);

    return EXIT_SUCCESS;
}