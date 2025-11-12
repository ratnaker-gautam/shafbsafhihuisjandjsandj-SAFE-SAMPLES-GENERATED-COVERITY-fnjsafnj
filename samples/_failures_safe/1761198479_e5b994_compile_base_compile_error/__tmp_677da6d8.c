//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10
#define MATRIX_ELEMENT_MAX 1000
#define MATRIX_ELEMENT_MIN -1000

#define MATRIX_OP_ADD 1
#define MATRIX_OP_SUBTRACT 2
#define MATRIX_OP_MULTIPLY 3

#define MATRIX_PRINT(matrix, rows, cols) do { \
    for (int i = 0; i < rows; i++) { \
        for (int j = 0; j < cols; j++) { \
            printf("%6d", matrix[i][j]); \
        } \
        printf("\n"); \
    } \
} while (0)

#define MATRIX_ZERO(matrix, rows, cols) do { \
    for (int i = 0; i < rows; i++) { \
        for (int j = 0; j < cols; j++) { \
            matrix[i][j] = 0; \
        } \
    } \
} while (0)

int read_int(const char* prompt, int min_val, int max_val) {
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
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)value;
    }
}

int read_matrix_element(int row, int col) {
    char prompt[64];
    snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", row, col);
    return read_int(prompt, MATRIX_ELEMENT_MIN, MATRIX_ELEMENT_MAX);
}

void read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_matrix_element(i, j);
        }
    }
}

int matrix_add(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], 
               int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long sum = (long)a[i][j] + (long)b[i][j];
            if (sum > INT_MAX || sum < INT_MIN) {
                return 0;
            }
            result[i][j] = (int)sum;
        }
    }
    return 1;
}

int matrix_subtract(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], 
                    int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long diff = (long)a[i][j] - (long)b[i][j];
            if (diff > INT_MAX || diff < INT_MIN) {
                return 0;
            }
            result[i][j] = (int)diff;
        }
    }
    return 1;
}

int matrix_multiply(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], 
                    int result[MAX_SIZE][MAX_SIZE], int rows1, int cols1, int cols2) {
    MATRIX_ZERO(result, rows1, cols2);
    
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            long sum = 0;
            for (int k = 0; k < cols1; k++) {
                long product = (long)a[i][k] * (long)b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    return 0;
                }
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    return 0;
                }
            }
            result[i][j] = (int)sum;
        }
    }
    return 1;
}

int main() {
    int rows1, cols1, rows2, cols2;
    int operation;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    
    printf("\nEnter values for matrix 1:\n");
    read_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    rows2 = read_int("\nEnter rows for matrix 2 (1-