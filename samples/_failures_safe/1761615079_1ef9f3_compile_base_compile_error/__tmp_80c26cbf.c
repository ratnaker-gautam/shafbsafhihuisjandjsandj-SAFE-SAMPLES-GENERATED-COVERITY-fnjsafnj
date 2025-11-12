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

#define MATRIX_INIT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                mat[i][j] = 0; \
            } \
        } \
    } while (0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                printf("%6.2f", mat[i][j]); \
            } \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(res, a, b, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                res[i][j] = a[i][j] + b[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULTIPLY(res, a, b, a_rows, a_cols, b_cols) \
    do { \
        for (int i = 0; i < a_rows; i++) { \
            for (int j = 0; j < b_cols; j++) { \
                res[i][j] = 0; \
                for (int k = 0; k < a_cols; k++) { \
                    res[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min_val, int max_val) {
    char input[MAX_INPUT_LEN];
    long val;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        
        if (input[strlen(input) - 1] != '\n') {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Input too long. Please try again.\n");
            continue;
        }
        
        val = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (val < min_val || val > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)val;
    }
}

int read_matrix(double mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    char input[MAX_INPUT_LEN];
    double val;
    char* endptr;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            while (1) {
                printf("Element [%d][%d]: ", i, j);
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    return 0;
                }
                
                if (input[strlen(input) - 1] != '\n') {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf("Input too long. Please try again.\n");
                    continue;
                }
                
                val = strtod(input, &endptr);
                if (endptr == input || *endptr != '\n') {
                    printf("Invalid input. Please enter a number.\n");
                    continue;
                }
                
                mat[i][j] = val;
                break;
            }
        }
    }
    return 1;
}

int main(void) {
    double matrix1[MAX_ROWS][MAX_COLS];
    double matrix2[MAX_ROWS][MAX_COLS];
    double result[MAX_ROWS][MAX_COLS];
    int rows1, cols1, rows2, cols2;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for first matrix (1-10): ", MIN_DIM, MAX_ROWS);
    if (rows1 == -1) return 1;
    
    cols1 = read_int("Enter columns for first matrix (1-10): ", MIN_DIM, MAX_COLS);
    if (cols1 == -1) return 1;
    
    if (!read_matrix(matrix1, rows1, cols1)) return 1;
    
    printf("\nFirst matrix:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    operation = read_int("\nSelect operation:\n1. Matrix Addition\n2. Matrix Multiplication\nChoice: ", 1, 2);
    if (operation == -1) return 1;
    
    if (operation == 1) {
        rows2 = read_int("Enter rows for second matrix (1-10): ", MIN_DIM, MAX_ROWS);
        if (rows2