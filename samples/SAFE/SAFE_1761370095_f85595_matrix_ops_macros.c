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

#define MATRIX_INPUT(m) do { \
    printf("Enter matrix elements row by row:\n"); \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            if (scanf("%d", &(m).data[i][j]) != 1) { \
                printf("Invalid input\n"); \
                return 1; \
            } \
        } \
    } \
} while(0)

int main() {
    Matrix mat1, mat2, result;
    int rows1, cols1, rows2, cols2;
    int operation;

    printf("Enter dimensions for first matrix (rows columns): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (!MATRIX_VALID_DIMS(rows1, cols1)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    MATRIX_INIT(mat1, rows1, cols1);
    MATRIX_INPUT(mat1);

    printf("Enter dimensions for second matrix (rows columns): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (!MATRIX_VALID_DIMS(rows2, cols2)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    MATRIX_INIT(mat2, rows2, cols2);
    MATRIX_INPUT(mat2);

    printf("Select operation (1: Addition, 2: Multiplication): ");
    if (scanf("%d", &operation) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    switch(operation) {
        case 1:
            MATRIX_ADD(mat1, mat2, result);
            printf("Matrix addition result:\n");
            MATRIX_PRINT(result);
            break;
        case 2:
            MATRIX_MULTIPLY(mat1, mat2, result);
            printf("Matrix multiplication result:\n");
            MATRIX_PRINT(result);
            break;
        default:
            printf("Invalid operation\n");
            return 1;
    }

    return 0;
}