//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

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
                printf("%3d ", mat[i][j]); \
            } \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(dest, src1, src2, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                if (src1[i][j] > 0 && src2[i][j] > INT_MAX - src1[i][j]) { \
                    printf("Integer overflow detected during addition\n"); \
                    return 1; \
                } \
                if (src1[i][j] < 0 && src2[i][j] < INT_MIN - src1[i][j]) { \
                    printf("Integer underflow detected during addition\n"); \
                    return 1; \
                } \
                dest[i][j] = src1[i][j] + src2[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULTIPLY(dest, src1, rows1, cols1, src2, rows2, cols2) \
    do { \
        for (int i = 0; i < rows1; i++) { \
            for (int j = 0; j < cols2; j++) { \
                int sum = 0; \
                for (int k = 0; k < cols1; k++) { \
                    if (src1[i][k] != 0 && src2[k][j] != 0) { \
                        if (src1[i][k] > 0) { \
                            if (src2[k][j] > INT_MAX / src1[i][k]) { \
                                printf("Integer overflow detected during multiplication\n"); \
                                return 1; \
                            } \
                        } else { \
                            if (src2[k][j] < INT_MIN / src1[i][k]) { \
                                printf("Integer overflow detected during multiplication\n"); \
                                return 1; \
                            } \
                        } \
                    } \
                    int product = src1[i][k] * src2[k][j]; \
                    if (product > 0 && sum > INT_MAX - product) { \
                        printf("Integer overflow detected during summation\n"); \
                        return 1; \
                    } \
                    if (product < 0 && sum < INT_MIN - product) { \
                        printf("Integer underflow detected during summation\n"); \
                        return 1; \
                    } \
                    sum += product; \
                } \
                dest[i][j] = sum; \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }
        
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d\n", min_val, max_val);
            continue;
        }
        
        break;
    }
    
    return value;
}

void read_matrix(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            mat[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main() {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = read_int("Enter number of rows for matrix 1 (1-10): ", MIN_DIM, MAX_ROWS);
    int cols1 = read_int("Enter number of columns for matrix 1 (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("\nEnter values for matrix 1:\n");
    read_matrix(matrix1, rows1, cols1);
    
    int rows2 = read_int("Enter number of rows for matrix 2 (1-10): ", MIN_D