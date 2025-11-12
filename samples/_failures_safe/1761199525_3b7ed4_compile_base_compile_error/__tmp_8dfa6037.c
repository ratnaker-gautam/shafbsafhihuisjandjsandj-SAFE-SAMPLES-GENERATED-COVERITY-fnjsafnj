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

#define MATRIX_ALLOCATE(rows, cols) (int**)malloc(sizeof(int*) * (rows))
#define MATRIX_FREE(matrix, rows) do { \
    for (int i = 0; i < (rows); i++) { \
        free((matrix)[i]); \
    } \
    free(matrix); \
} while (0)

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return a + b;
}

int safe_multiply(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return a * b;
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

int** matrix_create(int rows, int cols) {
    int** matrix = MATRIX_ALLOCATE(rows, cols);
    if (matrix == NULL) return NULL;
    
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(sizeof(int) * cols);
        if (matrix[i] == NULL) {
            MATRIX_FREE(matrix, i);
            return NULL;
        }
    }
    return matrix;
}

int** matrix_input(int rows, int cols) {
    int** matrix = matrix_create(rows, cols);
    if (matrix == NULL) return NULL;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, MATRIX_ELEMENT_MIN, MATRIX_ELEMENT_MAX);
        }
    }
    return matrix;
}

int** matrix_operation(int** a, int** b, int rows, int cols, int op) {
    int** result = matrix_create(rows, cols);
    if (result == NULL) return NULL;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            switch (op) {
                case MATRIX_OP_ADD:
                    result[i][j] = safe_add(a[i][j], b[i][j]);
                    break;
                case MATRIX_OP_SUBTRACT:
                    result[i][j] = safe_add(a[i][j], -b[i][j]);
                    break;
                case MATRIX_OP_MULTIPLY:
                    result[i][j] = safe_multiply(a[i][j], b[i][j]);
                    break;
                default:
                    result[i][j] = 0;
                    break;
            }
        }
    }
    return result;
}

int main() {
    int rows, cols;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    printf("\nEnter first matrix:\n");
    int** matrix1 = matrix_input(rows, cols);
    if (matrix1 == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    printf("\nEnter second matrix:\n");
    int** matrix2 = matrix_input(rows, cols);
    if (matrix2 == NULL) {
        printf("Memory allocation failed.\n