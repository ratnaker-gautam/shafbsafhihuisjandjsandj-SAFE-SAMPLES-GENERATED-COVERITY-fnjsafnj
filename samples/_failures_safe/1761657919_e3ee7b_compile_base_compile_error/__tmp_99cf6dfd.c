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

#define SAFE_READ_INT(prompt, var) do { \
    printf("%s", prompt); \
    if (scanf("%d", &(var)) != 1) { \
        printf("Invalid input\n"); \
        return 1; \
    } \
} while(0)

int main(void) {
    Matrix mat1, mat2, result;
    int rows1, cols1, rows2, cols2;
    int operation;

    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    SAFE_READ_INT("Select operation (1 or 2): ", operation);

    if (operation != 1 && operation != 2) {
        printf("Invalid operation selection\n");
        return 1;
    }

    printf("Enter dimensions for first matrix:\n");
    SAFE_READ_INT("Rows: ", rows1);
    SAFE_READ_INT("Columns: ", cols1);

    if (!MATRIX_VALID_DIMS(rows1, cols1)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }

    printf("Enter dimensions for second matrix:\n");
    SAFE_READ_INT("Rows: ", rows2);
    SAFE_READ_INT("Columns: ", cols2);

    if (!MATRIX_VALID_DIMS(rows2, cols2)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }

    MATRIX_INIT(mat1, rows1, cols1);
    MATRIX_INIT(mat2, rows2, cols2);

    printf("Enter elements for first matrix (%d x %d):\n", rows1, cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &mat1.data[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }

    printf("Enter elements for second matrix (%d x %d):\n", rows2, cols2);
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &mat2.data[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }

    printf("\nFirst matrix:\n");
    MATRIX_PRINT(mat1);
    printf("\nSecond matrix