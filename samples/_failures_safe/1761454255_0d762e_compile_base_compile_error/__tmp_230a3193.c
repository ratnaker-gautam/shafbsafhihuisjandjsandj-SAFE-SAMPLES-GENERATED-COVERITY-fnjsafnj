//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MAX_ELEMENTS (MAX_ROWS * MAX_COLS)
#define MAX_INPUT_LEN 32

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

int safe_str_to_int(const char* str, int* result) {
    if (str == NULL || result == NULL) return 0;
    char* endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return 0;
    if (val > INT_MAX || val < INT_MIN) return 0;
    *result = (int)val;
    return 1;
}

int read_matrix(Matrix* mat) {
    if (mat == NULL) return 0;
    
    char input[MAX_INPUT_LEN];
    
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    input[strcspn(input, "\n")] = '\0';
    if (!safe_str_to_int(input, &mat->rows)) return 0;
    if (mat->rows < MIN_DIM || mat->rows > MAX_ROWS) return 0;
    
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    input[strcspn(input, "\n")] = '\0';
    if (!safe_str_to_int(input, &mat->cols)) return 0;
    if (mat->cols < MIN_DIM || mat->cols > MAX_COLS) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) return 0;
            input[strcspn(input, "\n")] = '\0';
            if (!safe_str_to_int(input, &mat->data[i][j])) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix* mat) {
    if (mat == NULL) return;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->cols != b->rows) return 0;
    if (result->rows != a->rows || result->cols != b->cols) return 0;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) return 0;
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix* src, Matrix* dest) {
    if (src == NULL || dest == NULL) return 0;
    if (dest->rows != src->cols || dest->cols != src->rows) return 0;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    if (!read_matrix(&mat1)) {
        printf("Error reading first matrix\n");
        return 1;
    }
    
    printf("Enter second matrix:\n");
    if (!read_matrix(&mat2)) {
        printf("Error reading second matrix\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix multiplication:\n");
    if (mat1.cols == mat2.rows) {
        result.rows = mat1.rows;
        result.cols = mat2.cols;
        if (multiply_matrices(&mat1, &mat2, &result)) {
            print_matrix(&result);
        } else {
            printf("Multiplication failed: integer overflow or invalid dimensions\n");
        }
    } else {
        printf("Cannot multiply: incompatible dimensions\n");
    }
    
    printf("\nTrans