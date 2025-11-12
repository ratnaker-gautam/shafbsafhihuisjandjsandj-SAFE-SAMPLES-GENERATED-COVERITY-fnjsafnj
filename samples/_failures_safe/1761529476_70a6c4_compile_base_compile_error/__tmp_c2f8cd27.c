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
#define MATRIX_SET(m, i, j, val) ((m).data[(i)][(j)] = (val))

#define MATRIX_ADD(res, a, b) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions mismatch for addition\n"); \
        return 1; \
    } \
    (res).rows = (a).rows; \
    (res).cols = (a).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            int sum = (a).data[i][j] + (b).data[i][j]; \
            if (((a).data[i][j] > 0 && (b).data[i][j] > INT_MAX - (a).data[i][j]) || \
                ((a).data[i][j] < 0 && (b).data[i][j] < INT_MIN - (a).data[i][j])) { \
                printf("Integer overflow in addition\n"); \
                return 1; \
            } \
            (res).data[i][j] = sum; \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(res, a, b) do { \
    if ((a).cols != (b).rows) { \
        printf("Matrix dimensions mismatch for multiplication\n"); \
        return 1; \
    } \
    (res).rows = (a).rows; \
    (res).cols = (b).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int64_t sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                int64_t product = (int64_t)(a).data[i][k] * (int64_t)(b).data[k][j]; \
                if (product > INT_MAX || product < INT_MIN) { \
                    printf("Integer overflow in multiplication\n"); \
                    return 1; \
                } \
                sum += product; \
                if (sum > INT_MAX || sum < INT_MIN) { \
                    printf("Integer overflow in multiplication\n"); \
                    return 1; \
                } \
            } \
            (res).data[i][j] = (int)sum; \
        } \
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
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d\n", min, max);
            continue;
        }
        return value;
    }
}

void read_matrix(Matrix* m) {
    m->rows = read_int("Enter number of rows (1-10): ", 1, MAX_ROWS);
    m->cols = read_int("Enter number of columns (1-10): ", 1, MAX_COLS);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            m->data[i][j] = read_int(prompt, -10000, 10000);
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
    
    printf("\nEnter first matrix:\n");
    read_matrix