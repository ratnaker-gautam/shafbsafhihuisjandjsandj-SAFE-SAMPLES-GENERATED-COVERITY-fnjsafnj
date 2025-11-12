//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
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

int read_int(const char *prompt, int min, int max) {
    char buffer[100];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
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
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
            *(result + i * b_cols + j) = sum;
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    
    printf("Enter elements for matrix 1:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix1[i][j] = read_int(prompt, -1000, 1000);
        }
    }
    
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    
    printf("Enter elements for matrix 2:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix2[i][j] = read_int(prompt, -1000, 1000);
        }
    }
    
    printf("\nMatrix 1:\n");
    print_matrix((int *)matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix((int *)matrix2, rows2, cols2);
    
    printf("\nOperations:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    int choice = read_int("Select operation (1-2): ", 1, 2);
    
    if (choice == 1) {
        if (rows1 == rows2 && cols1 == cols2) {
            add_matrices((int *)matrix1, (int *)matrix2, (int *)result, rows1, cols1);
            printf("\nResult of addition:\n");
            print_matrix((int *)result, rows1, cols1);
        } else {
            printf("Error: Matrices must have same dimensions for addition.\n");
        }
    } else {
        if (cols1 == rows2) {
            multiply_matrices((int *)matrix1, (int *)matrix2, (int *)result, rows1, cols1, cols2);
            printf("\nResult of multiplication:\n");
            print_matrix((int *)result, rows1, cols2);
        } else {
            printf("Error: Columns of matrix 1 must equal rows of matrix 2 for multiplication.\n");
        }
    }
    
    return 0;
}