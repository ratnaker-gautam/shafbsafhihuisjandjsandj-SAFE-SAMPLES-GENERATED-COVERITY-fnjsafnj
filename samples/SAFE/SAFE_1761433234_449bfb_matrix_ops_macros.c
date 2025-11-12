//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_IDX(mat, i, j, cols) ((mat)[(i) * (cols) + (j)])

static int read_int_safe(int *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *value = (int)val;
    return 1;
}

static void print_matrix(int *mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", MATRIX_IDX(mat, i, j, cols));
        }
        printf("\n");
    }
}

static int matrix_multiply(int *a, int a_rows, int a_cols, int *b, int b_rows, int b_cols, int *result) {
    if (a_cols != b_rows) return 0;
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                long product = (long)MATRIX_IDX(a, i, k, a_cols) * (long)MATRIX_IDX(b, k, j, b_cols);
                if (product > INT_MAX || product < INT_MIN) return 0;
                sum += (int)product;
                if ((sum > 0 && product < 0 && sum < 0) || (sum < 0 && product > 0 && sum > 0)) return 0;
            }
            MATRIX_IDX(result, i, j, b_cols) = sum;
        }
    }
    return 1;
}

int main(void) {
    int a_rows, a_cols, b_rows, b_cols;
    printf("Enter first matrix rows and columns: ");
    if (!read_int_safe(&a_rows) || !read_int_safe(&a_cols)) {
        printf("Invalid input\n");
        return 1;
    }
    if (a_rows <= 0 || a_cols <= 0 || a_rows > MAX_DIM || a_cols > MAX_DIM) {
        printf("Invalid dimensions\n");
        return 1;
    }
    printf("Enter second matrix rows and columns: ");
    if (!read_int_safe(&b_rows) || !read_int_safe(&b_cols)) {
        printf("Invalid input\n");
        return 1;
    }
    if (b_rows <= 0 || b_cols <= 0 || b_rows > MAX_DIM || b_cols > MAX_DIM) {
        printf("Invalid dimensions\n");
        return 1;
    }
    int a[MAX_DIM * MAX_DIM];
    int b[MAX_DIM * MAX_DIM];
    printf("Enter first matrix elements:\n");
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < a_cols; j++) {
            if (!read_int_safe(&MATRIX_IDX(a, i, j, a_cols))) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    printf("Enter second matrix elements:\n");
    for (int i = 0; i < b_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            if (!read_int_safe(&MATRIX_IDX(b, i, j, b_cols))) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    int result[MAX_DIM * MAX_DIM] = {0};
    if (!matrix_multiply(a, a_rows, a_cols, b, b_rows, b_cols, result)) {
        printf("Matrix multiplication failed\n");
        return 1;
    }
    printf("Result matrix:\n");
    print_matrix(result, a_rows, b_cols);
    return 0;
}