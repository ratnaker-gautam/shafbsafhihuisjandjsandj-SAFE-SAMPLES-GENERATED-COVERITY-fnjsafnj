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
    MatrixOp operations[] = {
        matrix_add,
        matrix_multiply,
        matrix_transpose,
        matrix_print
    };
    
    const char *op_names[] = {
        "add",
        "multiply",
        "transpose",
        "print"
    };
    
    int matrix[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    
    printf("Enter matrix dimensions (rows cols): ");
    if (!read_matrix(&rows, &cols, matrix)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    printf("Available operations:\n");
    for (size_t i = 0; i < sizeof(op_names) / sizeof(op_names[0]); i++) {
        printf("%zu: %s\n", i, op_names[i]);
    }
    
    printf("Select operation (0-%zu): ", sizeof(op_names) / sizeof(op_names[0]) - 1);
    size_t choice;
    if (scanf("%zu", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice >= sizeof(operations) / sizeof(operations[0])) {
        printf("Invalid operation\n");
        return 1;
    }
    
    if (choice == 0 || choice == 1) {
        printf("Enter scalar: ");
    }
    
    operations[choice](rows, cols, matrix);
    
    if (choice != 3) {
        printf("Result:\n");
        matrix_print(rows, cols, matrix);
    }
    
    return 0;
}