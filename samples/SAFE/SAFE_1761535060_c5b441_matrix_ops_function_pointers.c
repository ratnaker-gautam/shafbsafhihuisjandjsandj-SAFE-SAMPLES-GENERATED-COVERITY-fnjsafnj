//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]);

void transpose(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    if (rows != cols || rows <= 0 || cols <= 0) return;
    for (int i = 0; i < rows; i++) {
        for (int j = i + 1; j < cols; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

void scale(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    if (rows <= 0 || cols <= 0) return;
    int factor;
    printf("Enter scaling factor: ");
    if (scanf("%d", &factor) != 1) return;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (factor > 0 && matrix[i][j] > INT_MAX / factor) matrix[i][j] = INT_MAX;
            else if (factor < 0 && matrix[i][j] < INT_MIN / factor) matrix[i][j] = INT_MIN;
            else matrix[i][j] *= factor;
        }
    }
}

void add_constant(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    if (rows <= 0 || cols <= 0) return;
    int constant;
    printf("Enter constant to add: ");
    if (scanf("%d", &constant) != 1) return;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (constant > 0 && matrix[i][j] > INT_MAX - constant) matrix[i][j] = INT_MAX;
            else if (constant < 0 && matrix[i][j] < INT_MIN - constant) matrix[i][j] = INT_MIN;
            else matrix[i][j] += constant;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    if (rows <= 0 || cols <= 0) return;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

int read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    printf("Enter %d elements row by row:\n", rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) return 0;
        }
    }
    return 1;
}

int main() {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    MatrixOp operations[] = {transpose, scale, add_constant};
    char* names[] = {"Transpose", "Scale", "Add Constant"};
    int num_ops = sizeof(operations) / sizeof(operations[0]);

    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2 || rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }

    if (!read_matrix(rows, cols, matrix)) {
        printf("Failed to read matrix\n");
        return 1;
    }

    printf("\nOriginal matrix:\n");
    print_matrix(rows, cols, matrix);

    int choice;
    printf("\nOperations:\n");
    for (int i = 0; i < num_ops; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    printf("Choose operation (1-%d): ", num_ops);
    
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > num_ops) {
        printf("Invalid choice\n");
        return 1;
    }

    operations[choice - 1](rows, cols, matrix);

    printf("\nResult matrix:\n");
    print_matrix(rows, cols, matrix);

    return 0;
}