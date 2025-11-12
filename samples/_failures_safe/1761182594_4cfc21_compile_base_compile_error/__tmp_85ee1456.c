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

#define MATRIX_GET(m, r, c) ((r) >= 0 && (r) < (m).rows && (c) >= 0 && (c) < (m).cols ? (m).data[r][c] : 0)
#define MATRIX_SET(m, r, c, v) do { \
    if ((r) >= 0 && (r) < (m).rows && (c) >= 0 && (c) < (m).cols) { \
        (m).data[r][c] = (v); \
    } \
} while(0)

#define MATRIX_INIT(m, r, c) do { \
    (m).rows = ((r) > 0 && (r) <= MAX_ROWS) ? (r) : 1; \
    (m).cols = ((c) > 0 && (c) <= MAX_COLS) ? (c) : 1; \
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

#define MATRIX_MULTIPLY(a, b, result) do { \
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
    int rows = read_int("Enter number of rows (1-10): ", 1, MAX_ROWS);
    int cols = read_int("Enter number of columns (1-10): ", 1, MAX_COLS);
    MATRIX_INIT(*m, rows, cols);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            m->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");