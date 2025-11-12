//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MAX_INPUT_LEN 20

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

int read_int(const char* prompt) {
    char input[MAX_INPUT_LEN + 1];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) return 0;
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
        if (safe_str_to_int(input, &value)) return value;
        printf("Invalid input. Please enter an integer.\n");
    }
}

void init_matrix(Matrix* mat) {
    if (mat == NULL) return;
    mat->rows = 0;
    mat->cols = 0;
    memset(mat->data, 0, sizeof(mat->data));
}

int validate_dimensions(int rows, int cols) {
    if (rows < MIN_DIM || rows > MAX_ROWS) return 0;
    if (cols < MIN_DIM || cols > MAX_COLS) return 0;
    return 1;
}

void input_matrix(Matrix* mat) {
    if (mat == NULL) return;
    int rows, cols;
    do {
        rows = read_int("Enter number of rows (1-10): ");
        cols = read_int("Enter number of columns (1-10): ");
    } while (!validate_dimensions(rows, cols));
    
    mat->rows = rows;
    mat->cols = cols;
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int("");
        }
    }
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
    if (!validate_dimensions(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix* src, Matrix* dest) {
    if (src == NULL || dest == NULL) return 0;
    if (!validate_dimensions(src->cols, src->rows)) return 0;
    
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    init_matrix(&mat1);
    init_matrix(&mat2);
    init_matrix(&result);
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("\nInput first matrix:\n");
    input_matrix(&mat1);
    
    printf("\nInput second matrix:\n");
    input_matrix(&mat2);
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix multiplication:\n");
    if (multiply_matrices(&mat1, &mat2, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot multiply matrices: dimension mismatch\n");
    }
    
    printf("\nTranspose of first matrix:\n");
    if (transpose_matrix(&mat1, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot transpose matrix\n");
    }
    
    printf("\nTranspose of second matrix:\n");