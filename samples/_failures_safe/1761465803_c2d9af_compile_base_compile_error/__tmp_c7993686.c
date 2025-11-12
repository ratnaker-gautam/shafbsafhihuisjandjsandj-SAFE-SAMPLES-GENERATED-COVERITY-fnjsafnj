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
        free(matrix[i]); \
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

int read_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val - 1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return value;
    }
}

int** create_matrix(int rows, int cols) {
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

void fill_matrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % (MATRIX_ELEMENT_MAX - MATRIX_ELEMENT_MIN + 1) + MATRIX_ELEMENT_MIN;
        }
    }
}

int** matrix_operation(int** a, int** b, int rows, int cols, int op) {
    int** result = create_matrix(rows, cols);
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

int main(void) {
    srand(42);
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    int rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    if (rows < 1) return 1;
    
    int cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    if (cols < 1) return 1;
    
    int** matrix_a = create_matrix(rows, cols);
    int** matrix_b = create_matrix(rows, cols);
    
    if (matrix_a == NULL || matrix_b == NULL) {
        printf("Memory allocation failed.\n");
        if (matrix_a) MATRIX_FREE(matrix_a, rows);
        if (matrix_b) MATRIX_FREE(matrix_b, rows);
        return 1;
    }
    
    fill_matrix(matrix_a, rows, cols);
    fill_matrix(matrix_b, rows, cols);
    
    printf("\nMatrix A:\n");
    MATRIX_PR