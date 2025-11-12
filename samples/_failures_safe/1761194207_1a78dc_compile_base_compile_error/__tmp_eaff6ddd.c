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
#define MATRIX_SET(m, i, j, val) do { \
    if ((i) >= 0 && (i) < (m).rows && (j) >= 0 && (j) < (m).cols) \
        (m).data[(i)][(j)] = (val); \
} while(0)

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions mismatch\n"); \
        return; \
    } \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            int sum = MATRIX_GET(a, i, j) + MATRIX_GET(b, i, j); \
            if ((sum > 0 && MATRIX_GET(a, i, j) > INT_MAX - MATRIX_GET(b, i, j)) || \
                (sum < 0 && MATRIX_GET(a, i, j) < INT_MIN - MATRIX_GET(b, i, j))) { \
                printf("Integer overflow detected\n"); \
                return; \
            } \
            MATRIX_SET(result, i, j, sum); \
        } \
    } \
    (result).rows = (a).rows; \
    (result).cols = (a).cols; \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        printf("Matrix dimensions incompatible for multiplication\n"); \
        return; \
    } \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                long long product = (long long)MATRIX_GET(a, i, k) * MATRIX_GET(b, k, j); \
                if (product > INT_MAX || product < INT_MIN) { \
                    printf("Integer overflow detected\n"); \
                    return; \
                } \
                long long new_sum = (long long)sum + product; \
                if (new_sum > INT_MAX || new_sum < INT_MIN) { \
                    printf("Integer overflow detected\n"); \
                    return; \
                } \
                sum = (int)new_sum; \
            } \
            MATRIX_SET(result, i, j, sum); \
        } \
    } \
    (result).rows = (a).rows; \
    (result).cols = (b).cols; \
} while(0)

#define MATRIX_PRINT(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%6d", MATRIX_GET(m, i, j)); \
        } \
        printf("\n"); \
    } \
} while(0)

void read_matrix(Matrix *m) {
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (scanf("%d", &m->rows) != 1 || m->rows < 1 || m->rows > MAX_ROWS) {
        printf("Invalid rows\n");
        exit(1);
    }
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (scanf("%d", &m->cols) != 1 || m->cols < 1 || m->cols > MAX_COLS) {
        printf("Invalid columns\n");
        exit(1);
    }
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) {
                printf("Invalid input\n");
                exit(1);
            }
        }
    }
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Enter first matrix:\n");
    read_matrix(&