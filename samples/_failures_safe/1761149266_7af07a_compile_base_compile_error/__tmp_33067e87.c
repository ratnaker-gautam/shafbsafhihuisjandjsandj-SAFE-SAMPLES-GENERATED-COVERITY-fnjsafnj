//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[rows][cols]);

void validate_matrix_params(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
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
        fprintf(stderr, "Transpose requires square matrix\n");
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
        fprintf(stderr, "Invalid scale factor\n");
        while (getchar() != '\n');
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scale_factor > 0 && matrix[i][j] > INT_MAX / scale_factor) {
                matrix[i][j] = INT_MAX;
            } else if (scale_factor < 0 && matrix[i][j] < INT_MIN / scale_factor) {
                matrix[i][j] = INT_MIN;
            } else {
                matrix[i][j] *= scale_factor;
            }
        }
    }
}

void add_constant(int rows, int cols, int matrix[rows][cols]) {
    validate_matrix_params(rows, cols);
    int constant;
    printf("Enter constant to add: ");
    if (scanf("%d", &constant) != 1) {
        fprintf(stderr, "Invalid constant\n");
        while (getchar() != '\n');
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (constant > 0 && matrix[i][j] > INT_MAX - constant) {
                matrix[i][j] = INT_MAX;
            } else if (constant < 0 && matrix[i][j] < INT_MIN - constant) {
                matrix[i][j] = INT_MIN;
            } else {
                matrix[i][j] += constant;
            }
        }
    }
}

void negate_matrix(int rows, int cols, int matrix[rows][cols]) {
    validate_matrix_params(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == INT_MIN) {
                matrix[i][j] = INT_MAX;
            } else {
                matrix[i][j] = -matrix[i][j];
            }
        }
    }
}

void execute_operation(MatrixOp op, int rows, int cols, int matrix[rows][cols]) {
    validate_matrix_params(rows, cols);
    if (op == NULL) {
        fprintf(stderr, "Invalid operation\n");
        return;
    }
    op(rows, cols, matrix);
}

int main(void) {
    int rows, cols;
    
    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &rows) != 1 || rows <= 0 || rows > MAX_SIZE) {
        fprintf(stderr, "Invalid row count\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter matrix columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &cols) != 1 || cols <= 0 || cols > MAX_SIZE) {
        fprintf(stderr, "Invalid column count\n");
        return EXIT_FAILURE;
    }
    
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Invalid matrix element\n");
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
        "Print",
        "Transpose",
        "Scale",
        "Add Constant",
        "Negate"
    };
    
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        printf("\n