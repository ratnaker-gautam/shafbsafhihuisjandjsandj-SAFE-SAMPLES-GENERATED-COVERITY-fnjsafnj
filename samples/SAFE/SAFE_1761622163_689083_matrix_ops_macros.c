//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_ELEM(mat, i, j, cols) ((mat)[(i) * (cols) + (j)])

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", MATRIX_ELEM(matrix, i, j, cols));
        }
        printf("\n");
    }
}

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("Enter dimensions for first matrix:\n");
    rows1 = read_int("Rows (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    printf("Enter dimensions for second matrix:\n");
    rows2 = read_int("Rows (1-10): ", 1, MAX_DIM);
    cols2 = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Matrix multiplication not possible: columns of first (%d) must equal rows of second (%d)\n", cols1, rows2);
        return 1;
    }
    
    int size1 = rows1 * cols1;
    int size2 = rows2 * cols2;
    int *matrix1 = malloc(size1 * sizeof(int));
    int *matrix2 = malloc(size2 * sizeof(int));
    int *result = malloc(rows1 * cols2 * sizeof(int));
    
    if (matrix1 == NULL || matrix2 == NULL || result == NULL) {
        printf("Memory allocation failed\n");
        free(matrix1);
        free(matrix2);
        free(result);
        return 1;
    }
    
    printf("Enter elements for first matrix (%d x %d):\n", rows1, cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            MATRIX_ELEM(matrix1, i, j, cols1) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
    
    printf("Enter elements for second matrix (%d x %d):\n", rows2, cols2);
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            MATRIX_ELEM(matrix2, i, j, cols2) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nSecond matrix:\n");
    print_matrix(matrix2, rows2, cols2);
    
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            int sum = 0;
            for (int k = 0; k < cols1; k++) {
                long long product = (long long)MATRIX_ELEM(matrix1, i, k, cols1) * (long long)MATRIX_ELEM(matrix2, k, j, cols2);
                if (product > 0 && sum > INT_MAX - product) {
                    sum = INT_MAX;
                    break;
                } else if (product < 0 && sum < INT_MIN - product) {
                    sum = INT_MIN;
                    break;
                }
                sum += (int)product;
            }
            MATRIX_ELEM(result, i, j, cols2) = sum;
        }
    }
    
    printf("\nResult of multiplication:\n");
    print_matrix(result, rows1, cols2);
    
    free(matrix1);
    free(matrix2);
    free(result);
    return 0;
}