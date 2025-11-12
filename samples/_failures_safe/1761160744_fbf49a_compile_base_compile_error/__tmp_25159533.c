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

#define MATRIX_INDEX(m, i, j) ((m).data[(i)][(j)])

#define MATRIX_SET(m, i, j, val) do { \
    if ((i) >= 0 && (i) < (m).rows && (j) >= 0 && (j) < (m).cols) \
        (m).data[(i)][(j)] = (val); \
} while(0)

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
                long sum = 0; \
                for (int k = 0; k < (a).cols; k++) { \
                    long product = (long)(a).data[i][k] * (long)(b).data[k][j]; \
                    if (product > INT_MAX - sum || product < INT_MIN + sum) { \
                        sum = 0; \
                        break; \
                    } \
                    sum += product; \
                } \
                (result).data[i][j] = (sum > INT_MAX || sum < INT_MIN) ? 0 : (int)sum; \
            } \
        } \
    } else { \
        (result).rows = 0; \
        (result).cols = 0; \
    } \
} while(0)

void matrix_print(Matrix m) {
    if (!MATRIX_VALID(m)) {
        printf("Invalid matrix\n");
        return;
    }
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%6d", m.data[i][j]);
        }
        printf("\n");
    }
}

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

void matrix_input(Matrix* m) {
    int rows, cols;
    
    rows = read_int("Enter number of rows (1-10): ", 1, MAX_ROWS);
    if (rows == -1) {
        printf("Invalid input for rows\n");
        m->rows = 0;
        m->cols = 0;
        return;
    }
    
    cols = read_int("Enter number of columns (1-10): ", 1, MAX_COLS);
    if (cols == -1) {
        printf("Invalid input for columns\n");
        m->rows = 0;
        m->cols = 0;
        return