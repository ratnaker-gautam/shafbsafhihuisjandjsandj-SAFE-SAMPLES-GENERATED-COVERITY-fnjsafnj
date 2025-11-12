//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[rows][cols]);

void matrix_add(int rows, int cols, int matrix[rows][cols]) {
    int scalar;
    if (scanf("%d", &scalar) != 1) {
        printf("Invalid input\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((scalar > 0 && matrix[i][j] > INT_MAX - scalar) ||
                (scalar < 0 && matrix[i][j] < INT_MIN - scalar)) {
                printf("Overflow detected\n");
                return;
            }
            matrix[i][j] += scalar;
        }
    }
}

void matrix_multiply(int rows, int cols, int matrix[rows][cols]) {
    int scalar;
    if (scanf("%d", &scalar) != 1) {
        printf("Invalid input\n");
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scalar != 0 && 
                ((matrix[i][j] > 0 && scalar > INT_MAX / matrix[i][j]) ||
                 (matrix[i][j] < 0 && scalar < INT_MAX / matrix[i][j]) ||
                 (matrix[i][j] > 0 && scalar < INT_MIN / matrix[i][j]) ||
                 (matrix[i][j] < 0 && scalar > INT_MIN / matrix[i][j]))) {
                printf("Overflow detected\n");
                return;
            }
            matrix[i][j] *= scalar;
        }
    }
}

void matrix_transpose(int rows, int cols, int matrix[rows][cols]) {
    if (rows != cols) {
        printf("Transpose requires square matrix\n");
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

void matrix_print(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void execute_operation(MatrixOp op, int rows, int cols, int matrix[rows][cols]) {
    op(rows, cols, matrix);
}

int main() {
    int rows, cols;
    
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    int matrix[MAX_SIZE][MAX_SIZE];
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid matrix element\n");
                return 1;
            }
        }
    }
    
    MatrixOp operations[] = {
        matrix_add,
        matrix_multiply,
        matrix_transpose,
        matrix_print
    };
    
    char* op_names[] = {
        "add",
        "multiply",
        "transpose",
        "print"
    };
    
    int num_ops = sizeof(operations) / sizeof(operations[0]);
    
    printf("Available operations:\n");
    for (int i = 0; i < num_ops; i++) {
        printf("%d: %s\n", i, op_names[i]);
    }
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice < 0 || choice >= num_ops) {
        printf("Invalid operation\n");
        return 1;
    }
    
    execute_operation(operations[choice], rows, cols, matrix);
    
    if (choice != 3) {
        matrix_print(rows, cols, matrix);
    }
    
    return 0;
}