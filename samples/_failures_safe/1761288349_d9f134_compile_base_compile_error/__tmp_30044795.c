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

#define MATRIX_ADD(a, b, res, r, c) \
    do { \
        for (int i = 0; i < r; i++) \
            for (int j = 0; j < c; j++) \
                res[i][j] = a[i][j] + b[i][j]; \
    } while (0)

#define MATRIX_MULT(a, b, res, r1, c1, c2) \
    do { \
        for (int i = 0; i < r1; i++) { \
            for (int j = 0; j < c2; j++) { \
                res[i][j] = 0; \
                for (int k = 0; k < c1; k++) \
                    res[i][j] += a[i][k] * b[k][j]; \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return (int)value;
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    int choice;
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows1 == -1) return 1;
    
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", MIN_DIM, MAX_COLS);
    if (cols1 == -1) return 1;
    
    printf("Enter elements for matrix 1:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix1[i][j] = read_int(prompt, INT_MIN, INT_MAX);
            if (matrix1[i][j] == -1) return 1;
        }
    }
    
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows2 == -1) return 1;
    
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", MIN_DIM, MAX_COLS);
    if (cols2 == -1) return 1;
    
    printf("Enter elements for matrix 2:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix2[i][j] = read_int(prompt, INT_MIN, INT_MAX);
            if (matrix2[i][j] == -1) return 1;
        }
    }
    
    while (1) {
        printf("\nMatrix Operations:\n");
        printf("1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Exit\n");
        
        choice = read_int("Select operation (1-3): ", 1, 3);
        if (choice == -1) return 1;
        
        if (choice == 3) break;
        
        if (choice == 1) {
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Matrix dimensions must match for addition.\n");
                continue;
            }
            
            MATRIX_ADD(matrix1, matrix2, result, rows1, cols1);
            printf("Result of addition:\n");
            MATRIX_PRINT(result, rows1, cols1);
        }
        else if (choice