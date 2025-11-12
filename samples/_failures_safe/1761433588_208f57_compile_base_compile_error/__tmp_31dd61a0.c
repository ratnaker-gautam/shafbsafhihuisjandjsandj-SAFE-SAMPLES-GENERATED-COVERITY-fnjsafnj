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

#define MATRIX_ADD(res, a, b, rows, cols) \
    do { \
        for (int i = 0; i < rows; i++) { \
            for (int j = 0; j < cols; j++) { \
                res[i][j] = a[i][j] + b[i][j]; \
            } \
        } \
    } while (0)

#define MATRIX_MULT(res, a, b, ra, ca, cb) \
    do { \
        for (int i = 0; i < ra; i++) { \
            for (int j = 0; j < cb; j++) { \
                res[i][j] = 0; \
                for (int k = 0; k < ca; k++) { \
                    res[i][j] += a[i][k] * b[k][j]; \
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
            return -1;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
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

int main(void) {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_ROWS][MAX_COLS];
    int matrix2[MAX_ROWS][MAX_COLS];
    int result[MAX_ROWS][MAX_COLS];

    printf("Matrix Operations Program\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    printf("3. Exit\n");

    while (1) {
        choice = read_int("Select operation (1-3): ", 1, 3);
        if (choice == 3) {
            break;
        }

        if (choice == 1) {
            printf("Matrix Addition\n");
            rows1 = read_int("Enter rows for matrix 1: ", MIN_DIM, MAX_ROWS);
            cols1 = read_int("Enter columns for matrix 1: ", MIN_DIM, MAX_COLS);
            rows2 = read_int("Enter rows for matrix 2: ", MIN_DIM, MAX_ROWS);
            cols2 = read_int("Enter columns for matrix 2: ", MIN_DIM, MAX_COLS);

            if (rows1 != rows2 || cols1 != cols2) {
                printf("Error: Matrices must have same dimensions for addition.\n");
                continue;
            }

            printf("Enter elements for matrix 1:\n");
            read_matrix(matrix1, rows1, cols1);
            printf("Enter elements for matrix 2:\n");
            read_matrix(matrix2, rows2, cols2);

            MATRIX_ADD(result, matrix1, matrix2, rows1, cols1);
            printf("Result of addition:\n");
            MATRIX_PRINT(result, rows1, cols1);
        } else if (choice == 2) {
            printf("Matrix Multiplication\n");
            rows1 = read_int("Enter rows for matrix 1: ", MIN_DIM, MAX_ROWS);
            cols1 = read_int("Enter columns for matrix 1: ", MIN_DIM, MAX_COLS);
            rows2 = read_int("Enter rows for matrix 2: ", MIN_DIM, MAX_ROWS);
            cols2 = read_int("Enter columns for matrix 2: ", MIN_DIM, MAX_COLS);

            if (cols1 != rows2) {
                printf("Error: Columns of matrix 1 must equal rows of matrix 2.\n");
                continue;
            }

            printf("Enter elements for matrix 1:\n");
            read_matrix(matrix1, rows1, cols1);
            printf("Enter elements for