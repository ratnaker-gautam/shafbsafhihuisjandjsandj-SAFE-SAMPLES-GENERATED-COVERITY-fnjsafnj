//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MAX_INPUT_LEN 100

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
                printf("%6d", m[i][j]); \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(a, b, r, c) \
    do { \
        for (int i = 0; i < r; i++) \
            for (int j = 0; j < c; j++) \
                a[i][j] += b[i][j]; \
    } while (0)

#define MATRIX_MULTIPLY(a, b, c, r1, c1, r2, c2) \
    do { \
        for (int i = 0; i < r1; i++) { \
            for (int j = 0; j < c2; j++) { \
                c[i][j] = 0; \
                for (int k = 0; k < c1; k++) \
                    c[i][j] += a[i][k] * b[k][j]; \
            } \
        } \
    } while (0)

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char input[MAX_INPUT_LEN];
    long val;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return min_val;
        }
        
        input[strcspn(input, "\n")] = 0;
        val = strtol(input, &endptr, 10);
        
        if (endptr == input || *endptr != '\0') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (val < min_val || val > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)val;
    }
}

void fill_matrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = get_valid_int(prompt, INT_MIN/2, INT_MAX/2);
        }
    }
}

int main(void) {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    int rows1, cols1, rows2, cols2;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    rows1 = get_valid_int("Enter rows for matrix 1 (1-10): ", MIN_DIM, MAX_ROWS);
    cols1 = get_valid_int("Enter columns for matrix 1 (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("Filling matrix 1:\n");
    fill_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    while (1) {
        printf("\nOperations:\n");
        printf("1. Matrix Addition\n");
        printf("2. Matrix Multiplication\n");
        printf("3. Exit\n");
        
        choice = get_valid_int("Choose operation: ", 1, 3);
        
        if (choice == 3) {
            break;
        }
        
        if (choice == 1) {
            rows2 = get_valid_int("Enter rows for matrix 2 (1-10): ", MIN_DIM, MAX_ROWS);
            cols2 = get_valid_int("Enter columns for matrix 2 (1-10): ", MIN_DIM, MAX_COLS);
            
            if (rows1 != rows2 || cols1 != cols2) {
                printf("Error: Matrices must have same dimensions for addition.\n");
                continue;
            }
            
            printf("Filling matrix 2:\n");
            fill_matrix(matrix2, rows2, cols2);
            
            MATRIX_INIT(result, rows1, cols1);
            for (int i = 0; i < rows1; i++) {
                for (int j = 0; j < cols1; j++) {
                    result[i][j] = matrix1[i][j];
                }
            }
            MATRIX_ADD(result, matrix2, rows1, cols1);
            
            printf("\nMatrix 1 + Matrix 2:\n");
            MATRIX_PRINT(result, rows1, cols1);
        }
        else if (choice == 2) {
            rows