//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MATRIX_MAX_SIZE 10
#define MATRIX_MIN_SIZE 1
#define MATRIX_OP_ADD 1
#define MATRIX_OP_SUBTRACT 2
#define MATRIX_OP_MULTIPLY 3

#define MATRIX_INPUT_PROMPT "Enter matrix size (rows cols): "
#define MATRIX_ELEM_PROMPT "Enter element [%d][%d]: "
#define MATRIX_OP_PROMPT "Choose operation (1=Add, 2=Subtract, 3=Multiply): "
#define MATRIX_RESULT_HEADER "Result matrix:"

#define SAFE_MUL(a, b, result) \
    do { \
        if ((b) != 0 && (a) > INT_MAX / (b)) { \
            *(result) = 0; \
            return 0; \
        } \
        *(result) = (a) * (b); \
    } while(0)

static int read_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[32];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            continue;
        }
        if (temp >= min_val && temp <= max_val) {
            value = (int)temp;
            break;
        }
    }
    return value;
}

static void read_matrix(int rows, int cols, int matrix[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), MATRIX_ELEM_PROMPT, i, j);
            matrix[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

static void print_matrix(int rows, int cols, int matrix[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

static int matrix_add(int rows, int cols, 
                     int a[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE],
                     int b[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE],
                     int result[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE]) {
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

static int matrix_subtract(int rows, int cols,
                          int a[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE],
                          int b[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE],
                          int result[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE]) {
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

static int matrix_multiply(int rows_a, int cols_a, int cols_b,
                          int a[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE],
                          int b[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE],
                          int result[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE]) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            int sum = 0;
            for (int k = 0; k < cols_a; k++) {
                int product;
                SAFE_MUL(a[i][k], b[k][j], &product);
                long new_sum = (long)sum + (long)product;
                if (new_sum > INT_MAX || new_sum < INT_MIN) {
                    return 0;
                }
                sum = (int)new_sum;
            }
            result[i][j] = sum;
        }
    }
    return 1;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf(MATRIX_INPUT_PROMPT);
    rows1 = read_int("", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    cols1 = read_int("", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    
    int matrix1[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
    int matrix2[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
    int