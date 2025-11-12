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
                if ((a).data[i][k] > 0 && (b).data[k][j] > 0) { \
                    if ((a).data[i][k] > INT_MAX / (b).data[k][j]) break; \
                } else if ((a).data[i][k] < 0 && (b).data[k][j] < 0) { \
                    if ((a).data[i][k] < INT_MAX / (b).data[k][j]) break; \
                } \
                sum += (a).data[i][k] * (b).data[k][j]; \
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

#define MATRIX_PRINT(m) do { \
    if (!MATRIX_VALID(m)) break; \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%6d", (m).data[i][j]); \
        } \
        printf("\n"); \
    } \
} while(0)

int safe_str_to_int(const char* str, int* result) {
    if (!str || !result) return 0;
    char* endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *result = (int)val;
    return 1;
}

int read_matrix_input(Matrix* m) {
    if (!m) return 0;
    
    char buffer[256];
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
    if (!safe_str_to_int(buffer, &m->rows)) return 0;
    
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
    if (!safe_str_to_int(buffer, &m->cols)) return 0;
    
    if (!MATRIX_VALID(*m)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
            if (!safe_str_to_int(buffer, &m->data[i][j])) return 0;
        }
    }
    return 1;
}

int main(void) {
    Matrix mat1, mat2, result;
    int choice;
    char buffer[256];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    printf("Enter choice (1-2): ");
    
    if (!fgets(buffer, sizeof(buffer), stdin)) return 1;
    if (!safe_str_to_int(buffer, &choice)) return 1;
    
    switch (choice) {
        case 1:
            printf("Enter