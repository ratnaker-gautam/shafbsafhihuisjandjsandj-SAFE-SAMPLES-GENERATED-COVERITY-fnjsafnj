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

#define MATRIX_MULTIPLY(a, b, result) do { \
    if (!MATRIX_VALID(a) || !MATRIX_VALID(b) || !MATRIX_VALID(result)) break; \
    if ((a).cols != (b).rows) break; \
    if ((result).rows != (a).rows || (result).cols != (b).cols) break; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                int product; \
                if (__builtin_smul_overflow((a).data[i][k], (b).data[k][j], &product)) { \
                    sum = (a).data[i][k] > 0 == (b).data[k][j] > 0 ? INT_MAX : INT_MIN; \
                    break; \
                } \
                if (__builtin_sadd_overflow(sum, product, &sum)) { \
                    sum = sum > 0 ? INT_MAX : INT_MIN; \
                    break; \
                } \
            } \
            (result).data[i][j] = sum; \
        } \
    } \
} while(0)

#define MATRIX_TRANSPOSE(m, result) do { \
    if (!MATRIX_VALID(m) || !MATRIX_VALID(result)) break; \
    if ((result).rows != (m).cols || (result).cols != (m).rows) break; \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            (result).data[j][i] = (m).data[i][j]; \
        } \
    } \
} while(0)

#define MATRIX_ADD(a, b, result) do { \
    if (!MATRIX_VALID(a) || !MATRIX_VALID(b) || !MATRIX_VALID(result)) break; \
    if ((a).rows != (b).rows || (a).cols != (b).cols) break; \
    if ((result).rows != (a).rows || (result).cols != (a).cols) break; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            if (__builtin_sadd_overflow((a).data[i][j], (b).data[i][j], &(result).data[i][j])) { \
                (result).data[i][j] = (a).data[i][j] > 0 ? INT_MAX : INT_MIN; \
            } \
        } \
    } \
} while(0)

#define MATRIX_PRINT(m) do { \
    if (!MATRIX_VALID(m)) break; \
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
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

void read_matrix(Matrix* m) {
    printf("Enter matrix dimensions:\n");
    m->rows = read_int("Rows (1-10): ", 1, MAX_ROWS);
    m->cols = read_int("Columns (1-10): ", 1, MAX_COLS);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i,