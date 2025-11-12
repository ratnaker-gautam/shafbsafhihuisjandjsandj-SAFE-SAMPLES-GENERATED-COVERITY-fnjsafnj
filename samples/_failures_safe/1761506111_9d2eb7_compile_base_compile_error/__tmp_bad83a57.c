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
                    dest[i][j] = INT_MAX; \
                } else if (src1[i][j] < 0 && src2[i][j] < INT_MIN - src1[i][j]) { \
                    dest[i][j] = INT_MIN; \
                } else { \
                    dest[i][j] = src1[i][j] + src2[i][j]; \
                } \
            } \
        } \
    } while (0)

#define MATRIX_MULTIPLY(dest, mat1, rows1, cols1, mat2, rows2, cols2) \
    do { \
        for (int i = 0; i < rows1; i++) { \
            for (int j = 0; j < cols2; j++) { \
                int64_t sum = 0; \
                for (int k = 0; k < cols1; k++) { \
                    if (mat1[i][k] != 0 && mat2[k][j] != 0) { \
                        if (mat1[i][k] > 0) { \
                            if (mat2[k][j] > INT_MAX / mat1[i][k]) { \
                                sum = INT_MAX; \
                                break; \
                            } \
                        } else { \
                            if (mat2[k][j] < INT_MIN / mat1[i][k]) { \
                                sum = INT_MIN; \
                                break; \
                            } \
                        } \
                    } \
                    sum += (int64_t)mat1[i][k] * (int64_t)mat2[k][j]; \
                    if (sum > INT_MAX) { \
                        sum = INT_MAX; \
                        break; \
                    } \
                    if (sum < INT_MIN) { \
                        sum = INT_MIN; \
                        break; \
                    } \
                } \
                dest[i][j] = (int)sum; \
            } \
        } \
    } while (0)

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min_val, max_val);
    }
}

void matrix_input(int mat[MAX_ROWS][MAX_COLS], int rows, int cols) {
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat[i][j] = get_valid_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int main(void) {
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = get_valid_int("Enter rows for matrix 1: ", MIN_DIM, MAX_ROWS);
    int cols1 = get_valid_int("Enter columns for matrix 1: ", MIN_DIM, MAX_COLS);
    
    printf("\nMatrix 1:\n");
    matrix_input(matrix1, rows1, cols1);
    
    int rows2 = get_valid_int("Enter rows for matrix 2: ", MIN_DIM, MAX_ROWS);
    int cols2 = get_valid_int("Enter columns for matrix 2: ", MIN_DIM, MAX_COLS);
    
    printf("\nMatrix 2:\n");
    matrix_input(matrix2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    MATRIX_PRINT(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");