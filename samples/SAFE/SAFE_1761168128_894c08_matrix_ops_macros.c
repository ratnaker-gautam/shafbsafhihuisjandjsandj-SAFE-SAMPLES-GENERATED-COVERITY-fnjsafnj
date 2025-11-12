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

#define MATRIX_VALID_DIMS(r, c) ((r) > 0 && (r) <= MAX_ROWS && (c) > 0 && (c) <= MAX_COLS)

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions mismatch for addition\n"); \
        return 1; \
    } \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            (result).data[i][j] = (a).data[i][j] + (b).data[i][j]; \
        } \
    } \
    (result).rows = (a).rows; \
    (result).cols = (a).cols; \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        printf("Matrix dimensions incompatible for multiplication\n"); \
        return 1; \
    } \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            (result).data[i][j] = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                (result).data[i][j] += (a).data[i][k] * (b).data[k][j]; \
            } \
        } \
    } \
    (result).rows = (a).rows; \
    (result).cols = (b).cols; \
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
    
    m->rows = rows;
    m->cols = cols;
    
    printf("Enter %d elements for matrix %s:\n", rows * cols, name);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int val = read_int("", INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid input\n");
                exit(1);
            }
            m->data[i][j] = val;
        }
    }
}

int main() {
    Matrix a, b, result;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    operation = read_int("Select operation (1-2): ", 1, 2);
    if (operation == -1) {
        printf("Invalid operation selection\n");
        return 1;
    }
    
    matrix_input(&a, "A");
    matrix_input(&b, "B");
    
    MATRIX_INIT(result, 0, 0);
    
    if (operation == 1) {
        MATRIX_ADD(a, b, result);
    } else {
        MATRIX_MULTIPLY(a, b, result);
    }
    
    printf("Result matrix:\n");
    MATRIX_PRINT(result);
    
    return 0;
}