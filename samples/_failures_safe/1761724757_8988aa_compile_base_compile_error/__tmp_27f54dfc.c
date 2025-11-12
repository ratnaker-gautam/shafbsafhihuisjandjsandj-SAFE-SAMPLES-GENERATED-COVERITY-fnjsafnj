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

#define MATRIX_VALID(m) ((m).rows > 0 && (m).rows <= MAX_ROWS && \
                        (m).cols > 0 && (m).cols <= MAX_COLS)

#define MATRIX_INDEX(m, i, j) ((i) >= 0 && (i) < (m).rows && \
                              (j) >= 0 && (j) < (m).cols)

#define MATRIX_SET(m, i, j, v) do { \
    if (MATRIX_INDEX(m, i, j)) { \
        (m).data[i][j] = (v); \
    } \
} while(0)

#define MATRIX_GET(m, i, j) (MATRIX_INDEX(m, i, j) ? (m).data[i][j] : 0)

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows == (b).rows && (a).cols == (b).cols && \
        MATRIX_VALID(a) && MATRIX_VALID(b)) { \
        (result).rows = (a).rows; \
        (result).cols = (a).cols; \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (a).cols; j++) { \
                int sum = (a).data[i][j] + (b).data[i][j]; \
                if (((a).data[i][j] > 0 && (b).data[i][j] > INT_MAX - (a).data[i][j]) || \
                    ((a).data[i][j] < 0 && (b).data[i][j] < INT_MIN - (a).data[i][j])) { \
                    (result).data[i][j] = 0; \
                } else { \
                    (result).data[i][j] = sum; \
                } \
            } \
        } \
    } else { \
        (result).rows = 0; \
        (result).cols = 0; \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols == (b).rows && MATRIX_VALID(a) && MATRIX_VALID(b)) { \
        (result).rows = (a).rows; \
        (result).cols = (b).cols; \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (b).cols; j++) { \
                int sum = 0; \
                for (int k = 0; k < (a).cols; k++) { \
                    long long product = (long long)(a).data[i][k] * (b).data[k][j]; \
                    if (product > INT_MAX || product < INT_MIN) { \
                        sum = 0; \
                        break; \
                    } \
                    sum += (int)product; \
                    if ((product > 0 && sum < 0) || (product < 0 && sum > 0)) { \
                        sum = 0; \
                        break; \
                    } \
                } \
                (result).data[i][j] = sum; \
            } \
        } \
    } else { \
        (result).rows = 0; \
        (result).cols = 0; \
    } \
} while(0)

#define MATRIX_PRINT(m) do { \
    if (MATRIX_VALID(m)) { \
        for (int i = 0; i < (m).rows; i++) { \
            for (int j = 0; j < (m).cols; j++) { \
                printf("%6d", (m).data[i][j]); \
            } \
            printf("\n"); \
        } \
    } else { \
        printf("Invalid matrix\n"); \
    } \
} while(0)

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return (int)value;
        }
        printf("Value must be between %d and %d.\n", min, max);
    }
}

void matrix_input(Matrix* m, const char* name) {
    printf("Enter dimensions for matrix