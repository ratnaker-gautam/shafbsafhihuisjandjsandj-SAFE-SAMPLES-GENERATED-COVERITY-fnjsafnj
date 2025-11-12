//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MATRIX_SIZE (MAX_ROWS * MAX_COLS)

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

#define MATRIX_GET(m, r, c) ((r) >= 0 && (r) < (m).rows && (c) >= 0 && (c) < (m).cols ? (m).data[r][c] : 0)
#define MATRIX_SET(m, r, c, v) do { \
    if ((r) >= 0 && (r) < (m).rows && (c) >= 0 && (c) < (m).cols) { \
        (m).data[r][c] = (v); \
    } \
} while(0)

#define MATRIX_INIT(m, r, c) do { \
    (m).rows = ((r) >= MIN_DIM && (r) <= MAX_ROWS) ? (r) : MIN_DIM; \
    (m).cols = ((c) >= MIN_DIM && (c) <= MAX_COLS) ? (c) : MIN_DIM; \
    memset((m).data, 0, sizeof((m).data)); \
} while(0)

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows == (b).rows && (a).cols == (b).cols) { \
        MATRIX_INIT((result), (a).rows, (a).cols); \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (a).cols; j++) { \
                int sum = (a).data[i][j] + (b).data[i][j]; \
                if (((a).data[i][j] > 0 && (b).data[i][j] > INT_MAX - (a).data[i][j]) || \
                    ((a).data[i][j] < 0 && (b).data[i][j] < INT_MIN - (a).data[i][j])) { \
                    sum = ((a).data[i][j] > 0) ? INT_MAX : INT_MIN; \
                } \
                (result).data[i][j] = sum; \
            } \
        } \
    } \
} while(0)

#define MATRIX_MULT(a, b, result) do { \
    if ((a).cols == (b).rows) { \
        MATRIX_INIT((result), (a).rows, (b).cols); \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (b).cols; j++) { \
                long sum = 0; \
                for (int k = 0; k < (a).cols; k++) { \
                    long product = (long)(a).data[i][k] * (long)(b).data[k][j]; \
                    if (product > INT_MAX - sum || product < INT_MIN - sum) { \
                        sum = (product > 0) ? INT_MAX : INT_MIN; \
                        break; \
                    } \
                    sum += product; \
                } \
                (result).data[i][j] = (sum > INT_MAX) ? INT_MAX : ((sum < INT_MIN) ? INT_MIN : (int)sum); \
            } \
        } \
    } \
} while(0)

#define MATRIX_PRINT(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%6d ", (m).data[i][j]); \
        } \
        printf("\n"); \
    } \
} while(0)

#define READ_INT(prompt, var, min, max) do { \
    int valid = 0; \
    while (!valid) { \
        printf(prompt); \
        if (scanf("%d", &(var)) == 1) { \
            if ((var) >= (min) && (var) <= (max)) { \
                valid = 1; \
            } else { \
                printf("Value must be between %d and %d\n", (min), (max)); \
            } \
        } else { \
            printf("Invalid input. Please enter an integer.\n"); \
            while (getchar() != '\n'); \
        } \
    } \
} while(0)

#define MATRIX_READ(m) do { \
    int r, c; \
    READ_INT("Enter number of rows (1-10): ", r, MIN_DIM, MAX_ROWS); \
    READ_INT("Enter number of columns (1-10): ", c, MIN_DIM, MAX_COLS); \
    MATRIX_INIT((m), r, c); \
    printf("Enter %d elements:\n", r * c); \
    for (int i = 0; i < r; i++) { \
        for (int j = 0; j < c; j++) { \
            READ_INT("", (m).data[i][j], INT_MIN, INT_MAX); \
        } \
    } \
} while(0