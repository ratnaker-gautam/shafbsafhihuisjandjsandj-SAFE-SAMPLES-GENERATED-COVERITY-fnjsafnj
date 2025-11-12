//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void print_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(int rows, int cols, int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            result[i][j] = a[i][j] + b[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

void multiply_matrices(int rows1, int cols1, int a[MAX_SIZE][MAX_SIZE], int rows2, int cols2, int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    if (cols1 != rows2) return;
    
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            int k = 0;
            while (k < cols1) {
                result[i][j] += a[i][k] * b[k][j];
                k++;
            }
        }
    }
}

void transpose_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    int i = 0;
    do {
        int j = 0;
        while (j < cols) {
            result[j][i] = matrix[i][j];
            j++;
        }
        i++;
    } while (i < rows);
}

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
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
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
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("3. Matrix Transpose\n");
    printf("4. Exit\n");
    
    while (1) {
        choice = read_int("Choose operation (1-4): ", 1, 4);
        
        if (choice == 4) {
            break;
        }
        
        switch (choice) {
            case 1: {
                rows1 = read_int("Enter rows for matrices (1-10): ", 1, MAX_SIZE);
                cols1 = read_int("Enter columns for matrices (1-10): ", 1, MAX_SIZE);
                
                printf("Matrix 1:\n");
                read_matrix(rows1, cols1, matrix1);
                printf("Matrix 2:\n");
                read_matrix(rows1, cols1, matrix2);
                
                add_matrices(rows1, cols1, matrix1, matrix2, result);
                printf("Result of addition:\n");
                print_matrix(rows1, cols1, result);
                break;
            }
            case 2: {
                rows1 = read_int("Enter rows for first matrix (1-10): ", 1, MAX_SIZE);
                cols1 = read_int("Enter columns for first matrix (1-10): ", 1, MAX_SIZE);
                rows2 = read_int("Enter rows for second matrix (1-10): ", 1, MAX_SIZE);
                cols2 = read_int("Enter columns for second matrix (1-10): ", 1, MAX_SIZE);
                
                if (cols1 != rows2) {
                    printf("Error: Columns of first matrix must equal rows of second matrix.\n");
                    break;
                }
                
                printf("Matrix 1:\n");
                read_matrix(rows1, cols1, matrix1);
                printf("Matrix 2:\n");
                read_matrix(rows2, cols2, matrix2);
                
                multiply_matrices(rows1, cols1, matrix1, rows2, cols2, matrix2, result);
                printf("Result of multiplication:\n");
                print_matrix(rows1, cols2, result);
                break;
            }
            case 3: {
                rows1 = read_int("Enter rows for matrix (1-10): ", 1, MAX_SIZE);
                cols