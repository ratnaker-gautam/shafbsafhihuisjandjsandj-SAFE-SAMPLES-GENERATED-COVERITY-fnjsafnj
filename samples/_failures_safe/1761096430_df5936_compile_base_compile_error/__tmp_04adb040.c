//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int rows, int cols, int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        while (j < cols) {
            dest[j][i] = src[i][j];
            j++;
        }
        i++;
    }
}

void multiply_matrices(int rows1, int cols1, int mat1[MAX_SIZE][MAX_SIZE], int rows2, int cols2, int mat2[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    if (cols1 != rows2) return;
    
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                if (mat1[i][k] > 0 && mat2[k][j] > 0) {
                    if (mat1[i][k] > INT_MAX / mat2[k][j]) continue;
                } else if (mat1[i][k] < 0 && mat2[k][j] < 0) {
                    if (mat1[i][k] < INT_MAX / mat2[k][j]) continue;
                } else if (mat1[i][k] > 0 && mat2[k][j] < 0) {
                    if (mat1[i][k] > INT_MIN / mat2[k][j]) continue;
                } else if (mat1[i][k] < 0 && mat2[k][j] > 0) {
                    if (mat1[i][k] < INT_MIN / mat2[k][j]) continue;
                }
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    printf("1. Transpose Matrix\n");
    printf("2. Multiply Matrices\n");
    
    choice = read_int("Select operation (1-2): ", 1, 2);
    
    if (choice == 1) {
        rows1 = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
        cols1 = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
        
        read_matrix(rows1, cols1, matrix1);
        
        printf("\nOriginal Matrix:\n");
        print_matrix(rows1, cols1, matrix1);
        
        memset(result, 0, sizeof(result));
        transpose_matrix(rows1, cols1, matrix1, result);
        
        printf("\nTransposed Matrix:\n");
        print_matrix(cols1, rows1, result);
        
    } else if (choice == 2) {
        rows1 = read_int("Enter rows for first matrix (1-10): ", 1, MAX_SIZE);
        cols1 = read_int("Enter columns for first matrix (1-10): ", 1, MAX_SIZE);
        
        rows2 = read_int("Enter rows for second matrix (1-10): ", 1, MAX_SIZE);
        cols2 = read_int("Enter columns for second matrix (1-10): ", 1, MAX_SIZE);
        
        if (cols1 != rows2) {
            printf("Error: Columns of first matrix must equal rows of second matrix.\n");
            return 1;
        }
        
        printf("First matrix