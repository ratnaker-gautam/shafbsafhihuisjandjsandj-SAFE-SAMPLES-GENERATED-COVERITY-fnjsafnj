//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef void (*MatrixOp)(int rows, int cols, int matrix[rows][cols]);

void add_scalar(int rows, int cols, int matrix[rows][cols]) {
    int scalar;
    printf("Enter scalar to add: ");
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

void multiply_scalar(int rows, int cols, int matrix[rows][cols]) {
    int scalar;
    printf("Enter scalar to multiply: ");
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
            matrix[i][j] *= scalar;
        }
    }
}

void transpose(int rows, int cols, int matrix[rows][cols]) {
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

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void fill_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Enter %d elements row by row:\n", rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input\n");
                return;
            }
        }
    }
}

int main() {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    
    printf("Enter matrix dimensions (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    fill_matrix(rows, cols, matrix);
    
    MatrixOp operations[] = {
        add_scalar,
        multiply_scalar,
        transpose,
        print_matrix
    };
    
    const char* operation_names[] = {
        "Add scalar",
        "Multiply scalar", 
        "Transpose",
        "Print matrix"
    };
    
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        printf("\nAvailable operations:\n");
        for (int i = 0; i < num_operations; i++) {
            printf("%d. %s\n", i + 1, operation_names[i]);
        }
        printf("0. Exit\n");
        printf("Choose operation: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid choice\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == 0) {
            break;
        }
        
        if (choice < 1 || choice > num_operations) {
            printf("Invalid choice\n");
            continue;
        }
        
        operations[choice - 1](rows, cols, matrix);
    }
    
    return 0;
}