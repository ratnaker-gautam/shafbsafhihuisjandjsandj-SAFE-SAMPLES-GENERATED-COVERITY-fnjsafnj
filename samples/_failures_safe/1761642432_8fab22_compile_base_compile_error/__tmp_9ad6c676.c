//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_INIT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                mat[i][j] = 0; \
            } \
        } \
    } while (0)

#define MATRIX_PRINT(mat, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                printf("%6d", mat[i][j]); \
            } \
            printf("\n"); \
        } \
    } while (0)

#define MATRIX_ADD(dest, src1, src2, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                if (src1[i][j] > 0 && src2[i][j] > INT_MAX - src1[i][j]) { \
                    printf("Integer overflow detected\n"); \
                    return 1; \
                } \
                if (src1[i][j] < 0 && src2[i][j] < INT_MIN - src1[i][j]) { \
                    printf("Integer underflow detected\n"); \
                    return 1; \
                } \
                dest[i][j] = src1[i][j] + src2[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULT(dest, mat1, r1, c1, mat2, r2, c2) \
    do { \
        for (int i = 0; i < r1; i++) { \
            for (int j = 0; j < c2; j++) { \
                dest[i][j] = 0; \
                for (int k = 0; k < c1; k++) { \
                    if (mat1[i][k] != 0 && mat2[k][j] != 0) { \
                        if (mat1[i][k] > 0) { \
                            if (mat2[k][j] > INT_MAX / mat1[i][k]) { \
                                printf("Integer overflow detected\n"); \
                                return 1; \
                            } \
                        } else { \
                            if (mat2[k][j] < INT_MIN / mat1[i][k]) { \
                                printf("Integer overflow detected\n"); \
                                return 1; \
                            } \
                        } \
                    } \
                    int product = mat1[i][k] * mat2[k][j]; \
                    if (dest[i][j] > 0 && product > INT_MAX - dest[i][j]) { \
                        printf("Integer overflow detected\n"); \
                        return 1; \
                    } \
                    if (dest[i][j] < 0 && product < INT_MIN - dest[i][j]) { \
                        printf("Integer underflow detected\n"); \
                        return 1; \
                    } \
                    dest[i][j] += product; \
                } \
            } \
        } \
    } while (0)

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
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
        break;
    }
    return value;
}

void read_matrix(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            mat[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    int choice;

    printf("Matrix Operations Program\n");
    printf("Enter dimensions for first matrix:\n");
    rows1 = read_int("Rows: ", MIN_DIM, MAX_ROWS);
    cols1 = read_int("Columns: ", MIN_DIM, MAX_COLS);

    printf("Enter first matrix elements:\n");
    read_matrix(matrix1, rows1, cols1);

    printf("Enter dimensions for second matrix:\n");
    rows2 = read_int("Rows: ", MIN_DIM, MAX_ROWS);
    cols2 = read_int("Columns: ", MIN_DIM, MAX_COLS);

    printf("Enter second matrix elements:\n");
    read_matrix(matrix2, rows