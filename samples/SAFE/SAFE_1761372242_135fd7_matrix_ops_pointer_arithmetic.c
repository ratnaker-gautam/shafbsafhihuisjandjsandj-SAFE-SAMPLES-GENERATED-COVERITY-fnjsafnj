//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void add_matrices(int *a, int *b, int *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(result + i * cols + j) = *(a + i * cols + j) + *(b + i * cols + j);
        }
    }
}

void multiply_matrices(int *a, int *b, int *result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            *(result + i * b_cols + j) = 0;
            for (int k = 0; k < a_cols; k++) {
                *(result + i * b_cols + j) += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
        }
    }
}

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    
    int matrix1[MAX_SIZE][MAX_SIZE];
    printf("Enter elements for matrix 1:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            matrix1[i][j] = read_int("", -1000, 1000);
        }
    }
    
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    
    int matrix2[MAX_SIZE][MAX_SIZE];
    printf("Enter elements for matrix 2:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            matrix2[i][j] = read_int("", -1000, 1000);
        }
    }
    
    printf("\nMatrix 1:\n");
    print_matrix((int *)matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix((int *)matrix2, rows2, cols2);
    
    printf("\nSelect operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    operation = read_int("Enter choice (1-2): ", 1, 2);
    
    if (operation == 1) {
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have same dimensions for addition.\n");
            return 1;
        }
        
        int result[MAX_SIZE][MAX_SIZE];
        add_matrices((int *)matrix1, (int *)matrix2, (int *)result, rows1, cols1);
        
        printf("\nResult of addition:\n");
        print_matrix((int *)result, rows1, cols1);
        
    } else if (operation == 2) {
        if (cols1 != rows2) {
            printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
            return 1;
        }
        
        int result[MAX_SIZE][MAX_SIZE];
        multiply_matrices((int *)matrix1, (int *)matrix2, (int *)result, rows1, cols1, cols2);
        
        printf("\nResult of multiplication:\n");
        print_matrix((int *)result, rows1, cols2);
    }
    
    return 0;
}