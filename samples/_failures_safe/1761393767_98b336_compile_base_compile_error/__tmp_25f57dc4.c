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
#define VALID_INDEX(r, c, mat) ((r) >= 0 && (r) < (mat).rows && (c) >= 0 && (c) < (mat).cols)

#define MATRIX_GET(mat, r, c) (VALID_INDEX((r), (c), (mat)) ? (mat).data[(r)][(c)] : 0)
#define MATRIX_SET(mat, r, c, val) do { \
    if (VALID_INDEX((r), (c), (mat))) { \
        (mat).data[(r)][(c)] = (val); \
    } \
} while(0)

#define MATRIX_ADD(res, a, b) do { \
    if ((a).rows == (b).rows && (a).cols == (b).cols) { \
        (res).rows = (a).rows; \
        (res).cols = (a).cols; \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (a).cols; j++) { \
                int sum = (a).data[i][j] + (b).data[i][j]; \
                if (((a).data[i][j] > 0 && (b).data[i][j] > INT_MAX - (a).data[i][j]) || \
                    ((a).data[i][j] < 0 && (b).data[i][j] < INT_MIN - (a).data[i][j])) { \
                    sum = (a).data[i][j] > 0 ? INT_MAX : INT_MIN; \
                } \
                (res).data[i][j] = sum; \
            } \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(res, a, b) do { \
    if ((a).cols == (b).rows) { \
        (res).rows = (a).rows; \
        (res).cols = (b).cols; \
        for (int i = 0; i < (a).rows; i++) { \
            for (int j = 0; j < (b).cols; j++) { \
                int64_t sum = 0; \
                for (int k = 0; k < (a).cols; k++) { \
                    sum += (int64_t)(a).data[i][k] * (int64_t)(b).data[k][j]; \
                    if (sum > INT_MAX || sum < INT_MIN) { \
                        sum = sum > 0 ? INT_MAX : INT_MIN; \
                        break; \
                    } \
                } \
                (res).data[i][j] = (int)sum; \
            } \
        } \
    } \
} while(0)

#define MATRIX_PRINT(mat) do { \
    for (int i = 0; i < (mat).rows; i++) { \
        for (int j = 0; j < (mat).cols; j++) { \
            printf("%6d", (mat).data[i][j]); \
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
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

void matrix_input(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    int rows = read_int("Rows (1-10): ", 1, MAX_ROWS);
    int cols = read_int("Columns (1-10): ", 1, MAX_COLS);
    
    mat->rows = rows;
    mat->cols = cols;
    
    printf("Enter %d elements for matrix %s:\n", rows * cols, name);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int("", INT_MIN, INT_MAX);
        }
    }
}

int main() {
    Matrix a = MATRIX_INIT;
    Matrix b = MATRIX_INIT;
    Matrix result = MATRIX_INIT;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    matrix_input(&a, "A