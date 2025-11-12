//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]);

void transpose(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    if (rows != cols || rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) return;
    for (int i = 0; i < rows; i++) {
        for (int j = i + 1; j < cols; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

void scale(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) return;
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
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) return;
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
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) return;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void apply_operation(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE], MatrixOp op) {
    if (op == NULL || rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) return;
    op(rows, cols, matrix);
}

int main() {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    printf("Enter %d matrix elements:\n", rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    MatrixOp operations[] = {transpose, scale, add_constant};
    char* names[] = {"Transpose", "Scale", "Add Constant"};
    int num_ops = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        printf("\nCurrent matrix:\n");
        print_matrix(rows, cols, matrix);
        
        printf("\nAvailable operations:\n");
        for (int i = 0; i < num_ops; i++) {
            printf("%d. %s\n", i + 1, names[i]);
        }
        printf("%d. Exit\n", num_ops + 1);
        
        int choice;
        printf("Select operation: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == num_ops + 1) break;
        if (choice < 1 || choice > num_ops) {
            printf("Invalid choice\n");
            continue;
        }
        
        apply_operation(rows, cols, matrix, operations[choice - 1]);
        
        if (choice == 1 && rows == cols) {
            int temp = rows;
            rows = cols;
            cols = temp;
        }
    }
    
    return 0;
}