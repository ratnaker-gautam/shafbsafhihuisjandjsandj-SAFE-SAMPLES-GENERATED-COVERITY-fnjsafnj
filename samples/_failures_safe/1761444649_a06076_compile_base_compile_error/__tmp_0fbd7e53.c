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

#define MATRIX_INDEX(m, i, j) ((i) >= 0 && (i) < (m).rows && (j) >= 0 && (j) < (m).cols ? (m).data[i][j] : 0)

#define MATRIX_SET(m, i, j, v) do { \
    if ((i) >= 0 && (i) < (m).rows && (j) >= 0 && (j) < (m).cols) { \
        (m).data[i][j] = (v); \
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
                    sum = (a).data[i][j] > 0 ? INT_MAX : INT_MIN; \
                } \
                (result).data[i][j] = sum; \
            } \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols == (b).rows) { \
        (result).rows = (a).rows; \
        (result).cols = (b).cols; \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (b).cols; j++) { \
                long sum = 0; \
                for (int k = 0; k < (a).cols; k++) { \
                    long product = (long)(a).data[i][k] * (long)(b).data[k][j]; \
                    if (product > INT_MAX - sum || product < INT_MIN + sum) { \
                        sum = product > 0 ? INT_MAX : INT_MIN; \
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
    printf("Enter matrix elements:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            m->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main() {
    Matrix a, b, result;
    int choice;
    char buffer[100];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("Enter choice (1-2): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error.\n