//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10
#define MATRIX_ELEM(matrix, i, j, cols) (matrix)[(i) * (cols) + (j)]
#define SWAP(a, b) do { int temp = (a); (a) = (b); (b) = temp; } while (0)

int read_int_safe(int *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *value = (int)val;
    return 1;
}

int main() {
    int rows, cols;
    printf("Enter matrix rows (1-%d): ", MAX_SIZE);
    if (!read_int_safe(&rows) || rows < 1 || rows > MAX_SIZE) {
        fprintf(stderr, "Invalid rows input\n");
        return 1;
    }
    printf("Enter matrix columns (1-%d): ", MAX_SIZE);
    if (!read_int_safe(&cols) || cols < 1 || cols > MAX_SIZE) {
        fprintf(stderr, "Invalid columns input\n");
        return 1;
    }

    int matrix[MAX_SIZE * MAX_SIZE];
    printf("Enter %d matrix elements:\n", rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int val;
            if (!read_int_safe(&val)) {
                fprintf(stderr, "Invalid matrix element\n");
                return 1;
            }
            MATRIX_ELEM(matrix, i, j, cols) = val;
        }
    }

    printf("Original matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", MATRIX_ELEM(matrix, i, j, cols));
        }
        printf("\n");
    }

    int transposed[MAX_SIZE * MAX_SIZE];
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            MATRIX_ELEM(transposed, i, j, rows) = MATRIX_ELEM(matrix, j, i, cols);
        }
    }

    printf("Transposed matrix:\n");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            printf("%d ", MATRIX_ELEM(transposed, i, j, rows));
        }
        printf("\n");
    }

    if (rows == cols) {
        for (int i = 0; i < rows; i++) {
            for (int j = i + 1; j < cols; j++) {
                SWAP(MATRIX_ELEM(matrix, i, j, cols), MATRIX_ELEM(matrix, j, i, cols));
            }
        }
        printf("In-place transposed matrix:\n");
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                printf("%d ", MATRIX_ELEM(matrix, i, j, cols));
            }
            printf("\n");
        }
    }

    int trace = 0;
    if (rows == cols) {
        for (int i = 0; i < rows; i++) {
            trace += MATRIX_ELEM(matrix, i, i, cols);
        }
        printf("Matrix trace: %d\n", trace);
    }

    return 0;
}