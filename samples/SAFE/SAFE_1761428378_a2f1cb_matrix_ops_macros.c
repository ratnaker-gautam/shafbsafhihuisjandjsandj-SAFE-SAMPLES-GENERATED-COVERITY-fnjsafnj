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

#define VALID_DIM(r, c) ((r) > 0 && (r) <= MAX_ROWS && (c) > 0 && (c) <= MAX_COLS)
#define VALID_INDEX(m, r, c) ((r) >= 0 && (r) < (m).rows && (c) >= 0 && (c) < (m).cols)

#define PRINT_MATRIX(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%4d", (m).data[i][j]); \
        } \
        printf("\n"); \
    } \
} while(0)

#define MATRIX_ADD(dest, src1, src2) do { \
    if ((src1).rows != (src2).rows || (src1).cols != (src2).cols) { \
        printf("Matrix dimensions mismatch\n"); \
        return 1; \
    } \
    (dest).rows = (src1).rows; \
    (dest).cols = (src1).cols; \
    for (int i = 0; i < (src1).rows; i++) { \
        for (int j = 0; j < (src1).cols; j++) { \
            (dest).data[i][j] = (src1).data[i][j] + (src2).data[i][j]; \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(dest, src1, src2) do { \
    if ((src1).cols != (src2).rows) { \
        printf("Matrix dimensions incompatible for multiplication\n"); \
        return 1; \
    } \
    (dest).rows = (src1).rows; \
    (dest).cols = (src2).cols; \
    for (int i = 0; i < (src1).rows; i++) { \
        for (int j = 0; j < (src2).cols; j++) { \
            (dest).data[i][j] = 0; \
            for (int k = 0; k < (src1).cols; k++) { \
                (dest).data[i][j] += (src1).data[i][k] * (src2).data[k][j]; \
            } \
        } \
    } \
} while(0)

int read_matrix(Matrix *m) {
    int rows, cols;
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (scanf("%d", &rows) != 1) return 0;
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (scanf("%d", &cols) != 1) return 0;
    
    if (!VALID_DIM(rows, cols)) return 0;
    
    m->rows = rows;
    m->cols = cols;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", &m->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

int main(void) {
    Matrix m1 = MATRIX_INIT;
    Matrix m2 = MATRIX_INIT;
    Matrix result = MATRIX_INIT;
    
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    if (!read_matrix(&m1)) {
        printf("Invalid input for first matrix\n");
        return 1;
    }
    
    printf("Enter second matrix:\n");
    if (!read_matrix(&m2)) {
        printf("Invalid input for second matrix\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    PRINT_MATRIX(m1);
    printf("\nSecond matrix:\n");
    PRINT_MATRIX(m2);
    
    printf("\nMatrix Addition:\n");
    MATRIX_ADD(result, m1, m2);
    PRINT_MATRIX(result);
    
    printf("\nMatrix Multiplication:\n");
    MATRIX_MULTIPLY(result, m1, m2);
    PRINT_MATRIX(result);
    
    return 0;
}