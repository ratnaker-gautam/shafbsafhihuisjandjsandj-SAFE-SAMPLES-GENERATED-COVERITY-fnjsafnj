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

#define MATRIX_ADD(a, b, result) do { \
    if ((a).rows != (b).rows || (a).cols != (b).cols) { \
        printf("Matrix dimensions mismatch for addition\n"); \
        return 1; \
    } \
    (result).rows = (a).rows; \
    (result).cols = (a).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            (result).data[i][j] = (a).data[i][j] + (b).data[i][j]; \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(a, b, result) do { \
    if ((a).cols != (b).rows) { \
        printf("Matrix dimensions mismatch for multiplication\n"); \
        return 1; \
    } \
    (result).rows = (a).rows; \
    (result).cols = (b).cols; \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            (result).data[i][j] = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                (result).data[i][j] += (a).data[i][k] * (b).data[k][j]; \
            } \
        } \
    } \
} while(0)

#define PRINT_MATRIX(mat) do { \
    for (int i = 0; i < (mat).rows; i++) { \
        for (int j = 0; j < (mat).cols; j++) { \
            printf("%6d", (mat).data[i][j]); \
        } \
        printf("\n"); \
    } \
} while(0)

int read_matrix(Matrix *mat) {
    char input[256];
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    int rows = atoi(input);
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    int cols = atoi(input);
    
    if (!VALID_DIMENSIONS(rows, cols)) {
        printf("Invalid matrix dimensions\n");
        return 0;
    }
    
    mat->rows = rows;
    mat->cols = cols;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) return 0;
            mat->data[i][j] = atoi(input);
        }
    }
    return 1;
}

int main(void) {
    Matrix mat1 = MATRIX_INIT;
    Matrix mat2 = MATRIX_INIT;
    Matrix result = MATRIX_INIT;
    
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    if (!read_matrix(&mat1)) {
        printf("Failed to read first matrix\n");
        return 1;
    }
    
    printf("Enter second matrix:\n");
    if (!read_matrix(&mat2)) {
        printf("Failed to read second matrix\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    PRINT_MATRIX(mat1);
    
    printf("\nSecond matrix:\n");
    PRINT_MATRIX(mat2);
    
    printf("\nMatrix addition:\n");
    MATRIX_ADD(mat1, mat2, result);
    PRINT_MATRIX(result);
    
    printf("\nMatrix multiplication:\n");
    MATRIX_MULTIPLY(mat1, mat2, result);
    PRINT_MATRIX(result);
    
    return 0;
}