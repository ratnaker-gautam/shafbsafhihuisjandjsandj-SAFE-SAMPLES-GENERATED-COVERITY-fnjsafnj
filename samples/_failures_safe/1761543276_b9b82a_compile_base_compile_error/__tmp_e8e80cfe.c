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

#define MATRIX_GET(m, i, j) ((i) >= 0 && (i) < (m).rows && (j) >= 0 && (j) < (m).cols ? (m).data[i][j] : INT_MIN)

#define MATRIX_SET(m, i, j, val) do { \
    if ((i) >= 0 && (i) < (m).rows && (j) >= 0 && (j) < (m).cols) { \
        (m).data[i][j] = (val); \
    } \
} while(0)

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows == (b).rows && (a).cols == (b).cols) { \
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
    if ((a).cols == (b).rows) { \
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
                    if ((sum > 0 && (int)product > INT_MAX - sum) || \
                        (sum < 0 && (int)product < INT_MIN - sum)) { \
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
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%6d", (m).data[i][j]); \
        } \
        printf("\n"); \
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
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

void read_matrix(Matrix* m) {
    m->rows = read_int("Enter number of rows (1-10): ", 1, MAX_ROWS);
    m->cols = read_int("Enter number of columns (1-10): ", 1, MAX_COLS);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            m->data[i][j] = read_int(prompt, -10000, 10000);
        }
    }
}

int main()