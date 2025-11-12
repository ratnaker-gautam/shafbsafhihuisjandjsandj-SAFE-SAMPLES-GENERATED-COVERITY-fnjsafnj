//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10
#define MATRIX_ELEMENT_MAX 1000
#define MATRIX_ELEMENT_MIN -1000

#define MATRIX_OP_ADD 1
#define MATRIX_OP_SUBTRACT 2
#define MATRIX_OP_MULTIPLY 3

#define SAFE_MULT(a, b, result) \
    do { \
        if ((a) > 0 && (b) > 0 && (a) > INT_MAX / (b)) { \
            *(result) = 0; \
            return 0; \
        } \
        if ((a) < 0 && (b) < 0 && (a) < INT_MAX / (b)) { \
            *(result) = 0; \
            return 0; \
        } \
        if ((a) > 0 && (b) < 0 && (b) < INT_MIN / (a)) { \
            *(result) = 0; \
            return 0; \
        } \
        if ((a) < 0 && (b) > 0 && (a) < INT_MIN / (b)) { \
            *(result) = 0; \
            return 0; \
        } \
        *(result) = (a) * (b); \
    } while(0)

#define MATRIX_INIT(matrix, rows, cols) \
    do { \
        for (int i = 0; i < (rows); i++) { \
            for (int j = 0; j < (cols); j++) { \
                (matrix)[i][j] = 0; \
            } \
        } \
    } while(0)

#define MATRIX_PRINT(matrix, rows, cols) \
    do { \
        for (int i = 0; i < (rows); i++) { \
            for (int j = 0; j < (cols); j++) { \
                printf("%6d", (matrix)[i][j]); \
            } \
            printf("\n"); \
        } \
    } while(0)

int read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int *rows, int *cols) {
    char input[256];
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", rows) != 1) return 0;
    if (*rows < 1 || *rows > MAX_SIZE) return 0;

    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", cols) != 1) return 0;
    if (*cols < 1 || *cols > MAX_SIZE) return 0;

    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < *rows; i++) {
        printf("Row %d: ", i + 1);
        if (fgets(input, sizeof(input), stdin) == NULL) return 0;
        char *token = strtok(input, " \t\n");
        for (int j = 0; j < *cols; j++) {
            if (token == NULL) return 0;
            int value;
            if (sscanf(token, "%d", &value) != 1) return 0;
            if (value < MATRIX_ELEMENT_MIN || value > MATRIX_ELEMENT_MAX) return 0;
            matrix[i][j] = value;
            token = strtok(NULL, " \t\n");
        }
    }
    return 1;
}

int matrix_add(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], 
               int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) ||
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                return 0;
            }
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return 1;
}

int matrix_subtract(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], 
                    int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((b[i][j] > 0 && a[i][j] < INT_MIN + b[i][j]) ||
                (b[i][j] < 0 && a[i][j] > INT_MAX + b[i][j])) {
                return 0;
            }
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return 1;
}

int matrix_multiply(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], 
                    int result[MAX_SIZE][MAX_SIZE], int rows1, int cols1, int cols2) {
    for (int i = 0; i < rows