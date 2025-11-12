//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_ELEM(mat, i, j, cols) ((mat)[(i) * (cols) + (j)])

void print_matrix(int *mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", MATRIX_ELEM(mat, i, j, cols));
        }
        printf("\n");
    }
}

int read_int_safe(int *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }
    *value = (int)val;
    return 1;
}

int main() {
    int rows1, cols1, rows2, cols2;
    printf("Enter first matrix dimensions (rows cols, max %d): ", MAX_DIM);
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    getchar();
    if (rows1 <= 0 || rows1 > MAX_DIM || cols1 <= 0 || cols1 > MAX_DIM) {
        printf("Invalid dimensions\n");
        return 1;
    }

    printf("Enter second matrix dimensions (rows cols, max %d): ", MAX_DIM);
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    getchar();
    if (rows2 <= 0 || rows2 > MAX_DIM || cols2 <= 0 || cols2 > MAX_DIM) {
        printf("Invalid dimensions\n");
        return 1;
    }

    if (cols1 != rows2) {
        printf("Matrix multiplication not possible\n");
        return 1;
    }

    int mat1[rows1 * cols1];
    int mat2[rows2 * cols2];
    int result[rows1 * cols2];

    printf("Enter first matrix elements:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (!read_int_safe(&MATRIX_ELEM(mat1, i, j, cols1))) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }

    printf("Enter second matrix elements:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (!read_int_safe(&MATRIX_ELEM(mat2, i, j, cols2))) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            int sum = 0;
            for (int k = 0; k < cols1; k++) {
                int a = MATRIX_ELEM(mat1, i, k, cols1);
                int b = MATRIX_ELEM(mat2, k, j, cols2);
                if ((a > 0 && b > INT_MAX / a) || (a < 0 && b < INT_MIN / a)) {
                    printf("Integer overflow detected\n");
                    return 1;
                }
                int product = a * b;
                if ((sum > 0 && product > INT_MAX - sum) || (sum < 0 && product < INT_MIN - sum)) {
                    printf("Integer overflow detected\n");
                    return 1;
                }
                sum += product;
            }
            MATRIX_ELEM(result, i, j, cols2) = sum;
        }
    }

    printf("First matrix:\n");
    print_matrix(mat1, rows1, cols1);
    printf("Second matrix:\n");
    print_matrix(mat2, rows2, cols2);
    printf("Result matrix:\n");
    print_matrix(result, rows1, cols2);

    return 0;
}