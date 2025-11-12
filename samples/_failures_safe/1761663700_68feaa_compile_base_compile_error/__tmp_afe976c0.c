//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MATRIX_SIZE (MAX_ROWS * MAX_COLS)

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

#define MATRIX_INIT(m, r, c) do { \
    (m).rows = (r); \
    (m).cols = (c); \
    memset((m).data, 0, sizeof((m).data)); \
} while(0)

#define MATRIX_GET(m, i, j) ((m).data[(i)][(j)])
#define MATRIX_SET(m, i, j, val) do { \
    if ((i) >= 0 && (i) < (m).rows && (j) >= 0 && (j) < (m).cols) \
        (m).data[(i)][(j)] = (val); \
} while(0)

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions mismatch\n"); \
        return 1; \
    } \
    (result).rows = (a).rows; \
    (result).cols = (a).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            int sum = MATRIX_GET(a, i, j) + MATRIX_GET(b, i, j); \
            if ((MATRIX_GET(a, i, j) > 0 && MATRIX_GET(b, i, j) > INT_MAX - MATRIX_GET(a, i, j)) || \
                (MATRIX_GET(a, i, j) < 0 && MATRIX_GET(b, i, j) < INT_MIN - MATRIX_GET(a, i, j))) { \
                printf("Integer overflow detected\n"); \
                return 1; \
            } \
            MATRIX_SET(result, i, j, sum); \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        printf("Matrix dimensions incompatible for multiplication\n"); \
        return 1; \
    } \
    (result).rows = (a).rows; \
    (result).cols = (b).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                long long product = (long long)MATRIX_GET(a, i, k) * MATRIX_GET(b, k, j); \
                if (product > INT_MAX || product < INT_MIN) { \
                    printf("Integer overflow detected\n"); \
                    return 1; \
                } \
                long long new_sum = (long long)sum + product; \
                if (new_sum > INT_MAX || new_sum < INT_MIN) { \
                    printf("Integer overflow detected\n"); \
                    return 1; \
                } \
                sum = (int)new_sum; \
            } \
            MATRIX_SET(result, i, j, sum); \
        } \
    } \
} while(0)

#define MATRIX_PRINT(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%6d", MATRIX_GET(m, i, j)); \
        } \
        printf("\n"); \
    } \
} while(0)

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    long value;
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (value < min || value > max) {
        return -1;
    }
    
    return (int)value;
}

void matrix_input(Matrix* m, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    int rows = read_int("Rows (1-10): ", 1, MAX_ROWS);
    int cols = read_int("Cols (1-10): ", 1, MAX_COLS);
    
    if (rows == -1 || cols == -1) {
        printf("Invalid dimensions\n");
        exit(1);
    }
    
    MATRIX_INIT(*m, rows, cols);
    
    printf("Enter %d elements for matrix %s:\n", rows * cols, name);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];