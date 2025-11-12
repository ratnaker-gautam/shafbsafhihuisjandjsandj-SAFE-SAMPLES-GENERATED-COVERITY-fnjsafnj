//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_ELEM(m, i, j, cols) ((m)[(i) * (cols) + (j)])

#define SWAP(a, b) do { \
    int temp = (a); \
    (a) = (b); \
    (b) = temp; \
} while (0)

static int read_int(const char *prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char *endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)value;
    }
}

static void read_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            MATRIX_ELEM(matrix, i, j, cols) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

static void print_matrix(const int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_ELEM(matrix, i, j, cols));
        }
        printf("\n");
    }
}

static void transpose_matrix(const int *src, int *dst, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_ELEM(dst, j, i, rows) = MATRIX_ELEM(src, i, j, cols);
        }
    }
}

static void multiply_matrices(const int *a, int a_rows, int a_cols,
                             const int *b, int b_rows, int b_cols,
                             int *result) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            long sum = 0;
            for (int k = 0; k < a_cols; k++) {
                long product = (long)MATRIX_ELEM(a, i, k, a_cols) * 
                              (long)MATRIX_ELEM(b, k, j, b_cols);
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    printf("Multiplication overflow detected.\n");
                    return;
                }
                sum += product;
            }
            MATRIX_ELEM(result, i, j, b_cols) = (int)sum;
        }
    }
}

static int find_determinant(int *matrix, int n) {
    if (n == 1) {
        return MATRIX_ELEM(matrix, 0, 0, n);
    }
    
    if (n == 2) {
        return MATRIX_ELEM(matrix, 0, 0, n) * MATRIX_ELEM(matrix, 1, 1, n) -
               MATRIX_ELEM(matrix, 0, 1, n) * MATRIX_ELEM(matrix, 1, 0, n);
    }
    
    int det = 0;
    int *submatrix = malloc((n-1) * (n-1) * sizeof(int));
    if (submatrix == NULL) {
        return 0;
    }
    
    for (int x = 0; x < n; x++) {
        int subi = 0;
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
                if (j == x) continue;
                MATRIX_ELEM(submatrix, subi, subj, n-1) = MATRIX_ELEM(matrix, i, j, n);
                subj++;
            }
            subi++;
        }
        
        int sign = (x % 2 == 0) ? 1 : -1;
        long term = (long)sign * (long)MATRIX_ELEM(matrix, 0, x, n) * 
                   (long)find_determinant(submatrix, n-1);
        if (term > INT_MAX - det || term < INT_MIN - det) {
            free(submatrix);
            return 0;
        }
        det += (int)term;
    }
    
    free(submatrix);
    return det;
}

int main(void) {
    int choice;
    int rows