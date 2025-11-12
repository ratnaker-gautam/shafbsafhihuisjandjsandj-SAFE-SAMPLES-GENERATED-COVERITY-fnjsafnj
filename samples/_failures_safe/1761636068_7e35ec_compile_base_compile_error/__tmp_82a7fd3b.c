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
                printf("%6d", mat[i][j]); \
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

#define MATRIX_MULT(res, a, b, a_rows, a_cols, b_cols) \
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

int safe_str_to_int(const char *str, int *result) {
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }
    *result = (int)val;
    return 1;
}

int read_matrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    char input[MAX_INPUT_LEN + 2];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) {
                return 0;
            }
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
            } else if (len == sizeof(input) - 1) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                return 0;
            }
            int value;
            if (!safe_str_to_int(input, &value)) {
                return 0;
            }
            matrix[i][j] = value;
        }
    }
    return 1;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    char input[MAX_INPUT_LEN + 2];
    
    printf("Enter number of rows for matrix A (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len == sizeof(input) - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    if (!safe_str_to_int(input, &rows1) || rows1 < MIN_DIM || rows1 > MAX_ROWS) {
        fprintf(stderr, "Invalid row count\n");
        return 1;
    }
    
    printf("Enter number of columns for matrix A (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len == sizeof(input) - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    if (!safe_str_to_int(input, &cols1) || cols1 < MIN_DIM || cols1 > MAX_COLS) {
        fprintf(stderr, "Invalid column count\n");
        return 1;
    }
    
    printf("Enter number of rows for matrix B (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");