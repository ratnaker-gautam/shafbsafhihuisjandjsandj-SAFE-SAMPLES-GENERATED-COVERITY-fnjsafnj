//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
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

void fill_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int("Enter element: ", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(int rows, int cols, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void multiply_matrices(int ra, int ca, int a[MAX_DIM][MAX_DIM], int rb, int cb, int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < ra; i++) {
        for (int j = 0; j < cb; j++) {
            result[i][j] = 0;
            for (int k = 0; k < ca; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void transpose_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    
    printf("Matrix Operations Program\n");
    
    while (1) {
        printf("\n1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Transpose matrix\n");
        printf("4. Exit\n");
        
        choice = read_int("Select operation: ", 1, 4);
        
        if (choice == 4) {
            break;
        }
        
        switch (choice) {
            case 1:
                rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
                cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
                rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
                cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
                
                if (rows1 != rows2 || cols1 != cols2) {
                    printf("Matrix dimensions must match for addition.\n");
                    break;
                }
                
                printf("Enter elements for matrix 1:\n");
                fill_matrix(rows1, cols1, matrix1);
                printf("Enter elements for matrix 2:\n");
                fill_matrix(rows2, cols2, matrix2);
                
                add_matrices(rows1, cols1, matrix1, matrix2, result);
                
                printf("Matrix 1:\n");
                print_matrix(rows1, cols1, matrix1);
                printf("Matrix 2:\n");
                print_matrix(rows2, cols2, matrix2);
                printf("Result:\n");
                print_matrix(rows1, cols1, result);
                break;
                
            case 2:
                rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
                cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
                rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
                cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
                
                if (cols1 != rows2) {
                    printf("Matrix 1 columns must equal matrix 2 rows for multiplication.\n");
                    break;
                }
                
                printf("Enter elements for matrix 1:\n");
                fill_matrix(rows1, cols1, matrix1);
                printf("Enter elements for matrix 2:\n");
                fill_matrix(rows2, cols2, matrix2);