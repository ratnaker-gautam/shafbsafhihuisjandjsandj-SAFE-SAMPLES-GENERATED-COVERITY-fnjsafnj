//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MAX_INPUT_LEN 100

#define MATRIX_INIT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) \
            for (int j = 0; j < cols; j++) \
                mat[i][j] = 0; \
    } while(0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) \
                printf("%6d", mat[i][j]); \
            printf("\n"); \
        } \
    } while(0)

#define MATRIX_ADD(dest, src1, src2, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) \
            for (int j = 0; j < cols; j++) \
                dest[i][j] = src1[i][j] + src2[i][j]; \
    } while(0)

#define MATRIX_MULT(dest, mat1, r1, c1, mat2, r2, c2) \
    do { \
        for (int i = 0; i < r1; i++) { \
            for (int j = 0; j < c2; j++) { \
                dest[i][j] = 0; \
                for (int k = 0; k < c1; k++) \
                    dest[i][j] += mat1[i][k] * mat2[k][j]; \
            } \
        } \
    } while(0)

int safe_str_to_int(const char *str, int *result) {
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *result = (int)val;
    return 1;
}

int read_matrix(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    char input[MAX_INPUT_LEN + 1];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fgets(input, sizeof(input), stdin) == NULL) return 0;
            input[strcspn(input, "\n")] = '\0';
            int value;
            if (!safe_str_to_int(input, &value)) return 0;
            mat[i][j] = value;
        }
    }
    return 1;
}

int main(void) {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    char input[MAX_INPUT_LEN + 1];
    int rows1, cols1, rows2, cols2;

    printf("Enter number of rows for matrix 1 (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    input[strcspn(input, "\n")] = '\0';
    if (!safe_str_to_int(input, &rows1)) return 1;
    if (rows1 < MIN_DIM || rows1 > MAX_ROWS) return 1;

    printf("Enter number of columns for matrix 1 (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    input[strcspn(input, "\n")] = '\0';
    if (!safe_str_to_int(input, &cols1)) return 1;
    if (cols1 < MIN_DIM || cols1 > MAX_COLS) return 1;

    printf("Enter matrix 1 elements (%d x %d):\n", rows1, cols1);
    if (!read_matrix(matrix1, rows1, cols1)) return 1;

    printf("Enter number of rows for matrix 2 (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    input[strcspn(input, "\n")] = '\0';
    if (!safe_str_to_int(input, &rows2)) return 1;
    if (rows2 < MIN_DIM || rows2 > MAX_ROWS) return 1;

    printf("Enter number of columns for matrix 2 (1-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;
    input[strcspn(input, "\n")] = '\0';
    if (!safe_str_to_int(input, &cols2)) return 1;
    if (cols2 < MIN_DIM || cols2 > MAX_COLS) return 1;

    printf("Enter matrix 2 elements (%d x %d):\n", rows2, cols2);
    if (!read_matrix(matrix2, rows2, cols2)) return 1;

    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);

    printf("\nMatrix 2:\n");
    MATRIX_PRINT(matrix2, rows2, cols2);

    printf("\nMatrix Addition