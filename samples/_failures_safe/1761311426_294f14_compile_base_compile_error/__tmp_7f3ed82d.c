//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_INIT(m, r, c) \
    do { \
        for (int i = 0; i < r; i++) \
            for (int j = 0; j < c; j++) \
                m[i][j] = 0; \
    } while (0)

#define MATRIX_PRINT(m, r, c) \
    do { \
        for (int i = 0; i < r; i++) { \
            for (int j = 0; j < c; j++) \
                printf("%3d ", m[i][j]); \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(a, b, r, c) \
    do { \
        for (int i = 0; i < r; i++) \
            for (int j = 0; j < c; j++) \
                a[i][j] += b[i][j]; \
    } while (0)

#define MATRIX_MULTIPLY(a, b, c, m, n, p) \
    do { \
        for (int i = 0; i < m; i++) { \
            for (int j = 0; j < p; j++) { \
                c[i][j] = 0; \
                for (int k = 0; k < n; k++) { \
                    if (a[i][k] > 0 && b[k][j] > 0) { \
                        if (a[i][k] > INT_MAX / b[k][j]) { \
                            printf("Integer overflow detected\n"); \
                            return 1; \
                        } \
                    } else if (a[i][k] < 0 && b[k][j] < 0) { \
                        if (a[i][k] < INT_MIN / b[k][j]) { \
                            printf("Integer overflow detected\n"); \
                            return 1; \
                        } \
                    } else if (a[i][k] < 0 && b[k][j] > 0) { \
                        if (a[i][k] < INT_MIN / b[k][j]) { \
                            printf("Integer overflow detected\n"); \
                            return 1; \
                        } \
                    } else if (a[i][k] > 0 && b[k][j] < 0) { \
                        if (a[i][k] > INT_MAX / b[k][j]) { \
                            printf("Integer overflow detected\n"); \
                            return 1; \
                        } \
                    } \
                    c[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        break;
    }
    return value;
}

void read_matrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

int main(void) {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("3. Exit\n");
    
    choice = read_int("Enter your choice (1-3): ", 1, 3);
    
    if (choice == 3) {
        return 0;
    }
    
    if (choice == 1) {
        printf("\nMatrix Addition\n");
        rows1 = read_int("Enter number of rows for matrix 1: ", MIN_DIM, MAX_ROWS);
        cols1 = read_int("Enter number of columns for matrix 1: ", MIN_DIM, MAX_COLS);
        
        rows2 = read_int("Enter number of rows for matrix 2: ", MIN_DIM, MAX_ROWS);
        cols2 = read_int("Enter number of columns for matrix 2: ", MIN_DIM, MAX_COLS);
        
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Error: Matrices must have the same dimensions for addition.\n");
            return 1;
        }
        
        printf("\nEnter elements for matrix 1:\n");