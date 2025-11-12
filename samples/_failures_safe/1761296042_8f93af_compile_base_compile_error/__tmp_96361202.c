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

#define MATRIX_INIT { .data = {{0}}, .rows = 0, .cols = 0 }

#define VALID_DIMENSIONS(r, c) ((r) > 0 && (r) <= MAX_ROWS && (c) > 0 && (c) <= MAX_COLS)
#define VALID_INDEX(m, r, c) ((r) >= 0 && (r) < (m).rows && (c) >= 0 && (c) < (m).cols)

#define MATRIX_GET(m, r, c) (VALID_INDEX(m, r, c) ? (m).data[r][c] : 0)
#define MATRIX_SET(m, r, c, v) do { if (VALID_INDEX(m, r, c)) (m).data[r][c] = (v); } while(0)

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions mismatch\n"); \
        (result).rows = 0; \
        (result).cols = 0; \
    } else { \
        (result).rows = (a).rows; \
        (result).cols = (a).cols; \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (a).cols; j++) { \
                int sum = (a).data[i][j] + (b).data[i][j]; \
                if (((a).data[i][j] > 0 && (b).data[i][j] > INT_MAX - (a).data[i][j]) || \
                    ((a).data[i][j] < 0 && (b).data[i][j] < INT_MIN - (a).data[i][j])) { \
                    printf("Integer overflow detected\n"); \
                    (result).rows = 0; \
                    (result).cols = 0; \
                    break; \
                } \
                (result).data[i][j] = sum; \
            } \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        printf("Matrix dimensions incompatible for multiplication\n"); \
        (result).rows = 0; \
        (result).cols = 0; \
    } else { \
        (result).rows = (a).rows; \
        (result).cols = (b).cols; \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (b).cols; j++) { \
                int sum = 0; \
                for (int k = 0; k < (a).cols; k++) { \
                    long long product = (long long)(a).data[i][k] * (b).data[k][j]; \
                    if (product > INT_MAX || product < INT_MIN) { \
                        printf("Integer overflow detected\n"); \
                        (result).rows = 0; \
                        (result).cols = 0; \
                        break; \
                    } \
                    sum += (int)product; \
                    if ((product > 0 && sum > INT_MAX - (int)product) || \
                        (product < 0 && sum < INT_MIN - (int)product)) { \
                        printf("Integer overflow detected\n"); \
                        (result).rows = 0; \
                        (result).cols = 0; \
                        break; \
                    } \
                } \
                (result).data[i][j] = sum; \
            } \
        } \
    } \
} while(0)

#define MATRIX_PRINT(m) do { \
    if ((m).rows == 0 || (m).cols == 0) { \
        printf("Invalid matrix\n"); \
    } else { \
        for (int i = 0; i < (m).rows; i++) { \
            for (int j = 0; j < (m).cols; j++) { \
                printf("%6d", (m).data[i][j]); \
            } \
            printf("\n"); \
        } \
    } \
} while(0)

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d\n", min, max);
    }
}

void matrix_input(Matrix* m) {
    m->rows = read_int("Enter number of rows (1-10): ", 1, MAX_ROWS);
    m->cols = read_int("Enter number of columns (1-10): ", 1, MAX