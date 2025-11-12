//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
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
        for (int i = 0; i < rows; i++) \
            for (int j = 0; j < cols; j++) \
                mat[i][j] = 0; \
    } while (0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) \
                printf("%6d ", mat[i][j]); \
            printf("\n"); \
        } \
    } while (0)

#define SAFE_ADD(a, b) \
    (((b) > 0) ? (((a) > INT_MAX - (b)) ? INT_MAX : (a) + (b)) : \
    (((a) < INT_MIN - (b)) ? INT_MIN : (a) + (b)))

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char input[MAX_INPUT_LEN];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return min_val;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        value = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\0') {
            continue;
        }
        
        if (value >= min_val && value <= max_val) {
            return (int)value;
        }
    }
}

void matrix_add(int mat1[MAX_ROWS][MAX_COLS], int mat2[MAX_ROWS][MAX_COLS], 
                int result[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = SAFE_ADD(mat1[i][j], mat2[i][j]);
        }
    }
}

void matrix_multiply(int mat1[MAX_ROWS][MAX_COLS], int mat2[MAX_ROWS][MAX_COLS], 
                     int result[MAX_ROWS][MAX_COLS], int rows1, int cols1, int cols2) {
    MATRIX_INIT(result, rows1, cols2);
    
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            long sum = 0;
            for (int k = 0; k < cols1; k++) {
                long product = (long)mat1[i][k] * (long)mat2[k][j];
                if (product > INT_MAX - sum) {
                    sum = INT_MAX;
                    break;
                }
                if (product < INT_MIN - sum) {
                    sum = INT_MIN;
                    break;
                }
                sum += product;
            }
            result[i][j] = (sum > INT_MAX) ? INT_MAX : ((sum < INT_MIN) ? INT_MIN : (int)sum);
        }
    }
}

void matrix_transpose(int mat[MAX_ROWS][MAX_COLS], int result[MAX_ROWS][MAX_COLS], 
                      int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = mat[i][j];
        }
    }
}

int main() {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = get_valid_int("Enter rows for matrix 1 (1-10): ", MIN_DIM, MAX_ROWS);
    int cols1 = get_valid_int("Enter columns for matrix 1 (1-10): ", MIN_DIM, MAX_COLS);
    
    printf("\nEnter elements for matrix 1:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix1[i][j] = get_valid_int(prompt, INT_MIN, INT_MAX);
        }
    }
    
    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    int operation = get_valid_int("\nChoose operation:\n1. Addition\n2. Multiplication\n3. Transpose\nChoice: ", 1, 3);
    
    switch (operation) {
        case 1: {
            int rows2 = get_valid_int("Enter rows for matrix 2 (1-10): ", MIN_DIM, MAX_ROWS);
            int