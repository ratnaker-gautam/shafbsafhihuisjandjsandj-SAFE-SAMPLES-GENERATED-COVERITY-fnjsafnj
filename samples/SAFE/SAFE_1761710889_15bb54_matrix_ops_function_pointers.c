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
            if (scalar != 0 && matrix[i][j] > INT_MAX / scalar) {
                printf("Integer overflow detected\n");
                return;
            }
            if (scalar != 0 && matrix[i][j] < INT_MIN / scalar) {
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

int read_matrix(int *rows, int *cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    if (scanf("%d %d", rows, cols) != 2) {
        return 0;
    }
    
    if (*rows <= 0 || *cols <= 0 || *rows > MAX_SIZE || *cols > MAX_SIZE) {
        return 0;
    }
    
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

int main(void) {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix dimensions (rows cols): ");
    if (!read_matrix(&rows, &cols, matrix)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    MatrixOp operations[] = {
        matrix_add,
        matrix_multiply,
        matrix_transpose,
        matrix_print
    };
    
    char *op_names[] = {
        "add",
        "multiply",
        "transpose",
        "print"
    };
    
    int num_ops = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        printf("\nAvailable operations: ");
        for (int i = 0; i < num_ops; i++) {
            printf("%s ", op_names[i]);
        }
        printf("\nEnter operation (or 'quit'): ");
        
        char input[16];
        if (scanf("%15s", input) != 1) {
            break;
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        int found = 0;
        for (int i = 0; i < num_ops; i++) {
            if (strcmp(input, op_names[i]) == 0) {
                operations[i](rows, cols, matrix);
                found = 1;
                break;
            }
        }
        
        if (!found) {
            printf("Unknown operation\n");
        }
    }
    
    return 0;
}