//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>

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

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return INT_MIN;
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

int** allocate_matrix(int rows, int cols) {
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

int** matrix_add(int** a, int** b, int rows, int cols) {
    int** result = allocate_matrix(rows, cols);
    if (result == NULL) return NULL;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long sum = (long)a[i][j] + (long)b[i][j];
            if (sum > MATRIX_ELEMENT_MAX || sum < MATRIX_ELEMENT_MIN) {
                MATRIX_FREE(result, rows);
                return NULL;
            }
            result[i][j] = (int)sum;
        }
    }
    return result;
}

int** matrix_subtract(int** a, int** b, int rows, int cols) {
    int** result = allocate_matrix(rows, cols);
    if (result == NULL) return NULL;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long diff = (long)a[i][j] - (long)b[i][j];
            if (diff > MATRIX_ELEMENT_MAX || diff < MATRIX_ELEMENT_MIN) {
                MATRIX_FREE(result, rows);
                return NULL;
            }
            result[i][j] = (int)diff;
        }
    }
    return result;
}

int** matrix_multiply(int** a, int** b, int rows_a, int cols_a, int cols_b) {
    int** result = allocate_matrix(rows_a, cols_b);
    if (result == NULL) return NULL;
    
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            long sum = 0;
            for (int k = 0; k < cols_a; k++) {
                long product = (long)a[i][k] * (long)b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    MATRIX_FREE(result, rows_a);
                    return NULL;
                }
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    MATRIX_FREE(result, rows_a);
                    return NULL;
                }
            }
            result[i][j] = (int)sum;
        }
    }
    return result;
}

int main() {
    srand((unsigned int)time(NULL));
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    if (rows1 == INT_MIN) {
        printf