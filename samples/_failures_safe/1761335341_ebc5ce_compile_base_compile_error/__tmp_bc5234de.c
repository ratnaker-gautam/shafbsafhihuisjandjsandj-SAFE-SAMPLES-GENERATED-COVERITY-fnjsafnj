//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            while (1) {
                printf("Element [%d][%d]: ", i, j);
                char buffer[32];
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    printf("Input error\n");
                    exit(1);
                }
                char *endptr;
                long val = strtol(buffer, &endptr, 10);
                if (endptr == buffer || *endptr != '\n') {
                    printf("Invalid input. Enter an integer: ");
                    continue;
                }
                if (val < INT_MIN || val > INT_MAX) {
                    printf("Value out of range. Try again: ");
                    continue;
                }
                matrix[i][j] = (int)val;
                break;
            }
        }
    }
}

void add_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], 
                  int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) ||
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                printf("Integer overflow detected\n");
                exit(1);
            }
            result[i][j] = a[i][j] + b[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE],
                      int result[MAX_SIZE][MAX_SIZE], int rows, int cols, int common) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int sum = 0;
            for (int k = 0; k < common; k++) {
                long product = (long)a[i][k] * b[k][j];
                if (product < INT_MIN || product > INT_MAX) {
                    printf("Multiplication overflow detected\n");
                    exit(1);
                }
                long new_sum = (long)sum + product;
                if (new_sum < INT_MIN || new_sum > INT_MAX) {
                    printf("Sum overflow detected\n");
                    exit(1);
                }
                sum = (int)new_sum;
            }
            result[i][j] = sum;
        }
    }
}

void transpose_matrix(int matrix[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE],
                     int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        while (j < cols) {
            result[j][i] = matrix[i][j];
            j++;
        }
        i++;
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Enter dimensions for first matrix (rows columns): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    getchar();
    
    if (rows1 <= 0 || rows1 > MAX_SIZE || cols1 <= 0 || cols1 > MAX_SIZE) {
        printf("Invalid dimensions. Must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows columns): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    getchar();
    
    if (rows2 <= 0 || rows2 > MAX_SIZE || cols2 <= 0 || cols2 > MAX_SIZE) {
        printf("Invalid dimensions. Must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    read_matrix(matrix1, rows1, cols1);
    
    printf("\nSecond matrix:\n");
    read_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix operations:\n");
    printf("1. Addition\n");
    printf("2. Multiplication\n");
    printf("3. Transpose first matrix\n");
    printf("4. Transpose second matrix\n");
    printf("Choose operation (1-4): ");
    
    char choice_str[4];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    int choice;
    char *