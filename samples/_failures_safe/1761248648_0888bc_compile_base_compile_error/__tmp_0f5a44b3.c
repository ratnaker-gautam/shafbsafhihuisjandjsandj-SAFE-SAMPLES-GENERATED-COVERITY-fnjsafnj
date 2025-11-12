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
            printf("%d ", (m).data[i][j]); \
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

void matrix_fill(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            int val = read_int(prompt, -1000, 1000);
            if (val == -1) {
                printf("Invalid input, using 0\n");
                val = 0;
            }
            m->data[i][j] = val;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    int rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_ROWS);
    int cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_COLS);
    if (rows1 == -1 || cols1 == -1) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    MATRIX_INIT(mat1, rows1, cols1);
    printf("Fill matrix 1:\n");
    matrix_fill(&mat1);
    
    printf("Matrix 1:\n");
    MATRIX_PRINT(mat1);
    
    while (1) {
        printf("\nOperations:\n");
        printf("1. Matrix Multiplication\n");
        printf("2. Matrix Transpose\n");
        printf("3.