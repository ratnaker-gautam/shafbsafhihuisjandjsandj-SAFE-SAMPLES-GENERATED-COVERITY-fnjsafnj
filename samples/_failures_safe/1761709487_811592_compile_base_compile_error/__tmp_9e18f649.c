//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
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

int read_int_safe(const char* prompt, int min_val, int max_val) {
    char input[MAX_INPUT_LEN + 2];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            continue;
        }
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        } else if (len == sizeof(input) - 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Input too long\n");
            continue;
        }
        if (!safe_str_to_int(input, &value)) {
            printf("Invalid integer\n");
            continue;
        }
        if (value >= min_val && value <= max_val) {
            return value;
        }
        printf("Value must be between %d and %d\n", min_val, max_val);
    }
}

void matrix_init(Matrix* mat) {
    if (mat == NULL) return;
    mat->rows = 0;
    mat->cols = 0;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_read(Matrix* mat) {
    if (mat == NULL) return;
    printf("Enter matrix dimensions:\n");
    mat->rows = read_int_safe("Rows: ", MIN_DIM, MAX_ROWS);
    mat->cols = read_int_safe("Cols: ", MIN_DIM, MAX_COLS);
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int_safe("", INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* mat) {
    if (mat == NULL) return;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->cols != b->rows) return 0;
    if (a->rows > MAX_ROWS || b->cols > MAX_COLS) return 0;
    result->rows = a->rows;
    result->cols = b->cols;
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * (long long)b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) return 0;
                long long new_sum = (long long)sum + product;
                if (new_sum > INT_MAX || new_sum < INT_MIN) return 0;
                sum = (int)new_sum;
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix* mat, Matrix* result) {
    if (mat == NULL || result == NULL) return 0;
    if (mat->rows > MAX_COLS || mat->cols > MAX_ROWS) return 0;
    result->rows = mat->cols;
    result->cols = mat->rows;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[j][i] = mat->data[i][j];
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    matrix_init(&mat1);
    matrix_init(&mat2);
    matrix_init(&result);
    
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    matrix_read(&mat1);
    
    printf("Enter second matrix:\n");
    matrix_read(&mat2);
    
    printf("\nFirst matrix:\n");
    matrix_print(&mat1);
    
    printf("\nSecond matrix:\n");
    matrix_print(&mat