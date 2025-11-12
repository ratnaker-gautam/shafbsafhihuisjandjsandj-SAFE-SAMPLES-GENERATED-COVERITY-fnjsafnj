//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Operations Program\n");
    printf("Matrix dimensions must be between 1 and %d\n", MAX_DIM);
    
    rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
    
    printf("Enter elements for matrix 1:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix1[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
    
    rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
    
    printf("Enter elements for matrix 2:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix2[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
    
    printf("\nMatrix 1:\n");
    print_matrix(rows1, cols1, matrix1);
    
    printf("\nMatrix 2:\n");
    print_matrix(rows2, cols2, matrix2);
    
    printf("\nAvailable operations:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("3. Matrix Transposition\n");
    
    int choice = read_int("Select operation (1-3): ", 1, 3);
    
    switch (choice) {
        case 1:
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Error: Matrices must have same dimensions for addition.\n");
                return 1;
            }
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    if ((matrix1[i][j] > 0 && matrix2[i][j] > INT_MAX - matrix1[i][j]) ||
                        (matrix1[i][j] < 0 && matrix2[i][j] < INT_MIN - matrix1[i][j])) {
                        printf("Error: Integer overflow in addition.\n");
                        return 1;
                    }
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                }
            }
            printf("\nMatrix Addition Result:\n");
            print_matrix(rows1, cols1, result);
            break;
            
        case 2:
            if (cols1 != rows2) {
                printf("Error: Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
                return 1;
            }
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols2; j++) {
                    result[i][j] = 0;
                    for (int k = 0; k < cols1; k++) {
                        long long product = (long long)matrix1[i][k] * matrix2[k][j];
                        if (product > INT_MAX || product < INT_MIN) {
                            printf("Error: Integer overflow in multiplication.\n");
                            return 1;
                        }
                        if ((result[i][j] > 0 && product > INT_MAX - result[i][j]) ||
                            (result[i][j] < 0 && product < INT_MIN - result[i][j])) {
                            printf("Error: Integer overflow in summation.\n");
                            return 1;
                        }
                        result[i][j] += (int)product;
                    }
                }
            }
            printf("\nMatrix Multiplication Result:\n");
            print_matrix(rows1, cols2, result);
            break;
            
        case 3:
            for (int i = 0; i < cols1