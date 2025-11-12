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
                printf("%6.2f ", mat[i][j]); \
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

#define MATRIX_MULT(res, a, b, ra, ca, cb) \
    do { \
        for (int i = 0; i < ra; i++) { \
            for (int j = 0; j < cb; j++) { \
                res[i][j] = 0; \
                for (int k = 0; k < ca; k++) { \
                    res[i][j] += a[i][k] * b[k][j]; \
                } \
            } \
        } \
    } while (0)

int safe_str_to_int(const char* str, int* result) {
    char* endptr;
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

int read_matrix(double mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    char buffer[MAX_INPUT_LEN];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                return 0;
            }
            buffer[strcspn(buffer, "\n")] = '\0';
            char* endptr;
            double val = strtod(buffer, &endptr);
            if (endptr == buffer || *endptr != '\0') {
                return 0;
            }
            mat[i][j] = val;
        }
    }
    return 1;
}

int main(void) {
    double matrix_a[MAX_ROWS][MAX_COLS];
    double matrix_b[MAX_ROWS][MAX_COLS];
    double result[MAX_ROWS][MAX_COLS];
    char input[MAX_INPUT_LEN];
    int rows_a, cols_a, rows_b, cols_b;
    int choice;

    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("Enter choice (1 or 2): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    if (!safe_str_to_int(input, &choice) || (choice != 1 && choice != 2)) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }

    printf("Enter rows for matrix A (1-%d): ", MAX_ROWS);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    if (!safe_str_to_int(input, &rows_a) || rows_a < MIN_DIM || rows_a > MAX_ROWS) {
        fprintf(stderr, "Invalid rows\n");
        return 1;
    }

    printf("Enter columns for matrix A (1-%d): ", MAX_COLS);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    if (!safe_str_to_int(input, &cols_a) || cols_a < MIN_DIM || cols_a > MAX_COLS) {
        fprintf(stderr, "Invalid columns\n");
        return 1;
    }

    if (choice == 1) {
        rows_b = rows_a;
        cols_b = cols_a;
    } else {
        printf("Enter rows for matrix B (1-%d): ", MAX_ROWS);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            return 1;
        }
        if (!safe_str_to_int(input, &rows_b) || rows_b < MIN_DIM || rows_b > MAX_ROWS) {
            fprintf(stderr, "Invalid rows\n");
            return 1;
        }

        printf("Enter columns for matrix B (1-%