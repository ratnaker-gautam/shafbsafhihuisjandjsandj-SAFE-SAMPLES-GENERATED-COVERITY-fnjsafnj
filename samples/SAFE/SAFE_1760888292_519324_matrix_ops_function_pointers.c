//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
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
                printf("Integer overflow detected\n");
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
                printf("Integer overflow detected\n");
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

void process_matrix(MatrixOp operation, int rows, int cols, int matrix[rows][cols]) {
    operation(rows, cols, matrix);
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
    
    printf("Original matrix:\n");
    matrix_print(rows, cols, matrix);
    
    MatrixOp operations[] = {
        matrix_add,
        matrix_multiply,
        matrix_transpose,
        matrix_print
    };
    
    char* op_names[] = {
        "Add scalar",
        "Multiply by scalar", 
        "Transpose",
        "Print"
    };
    
    int choice;
    printf("Choose operation (0-Add, 1-Multiply, 2-Transpose, 3-Print): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice < 0 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    
    process_matrix(operations[choice], rows, cols, matrix);
    
    if (choice != 3) {
        printf("Result:\n");
        matrix_print(rows, cols, matrix);
    }
    
    return 0;
}