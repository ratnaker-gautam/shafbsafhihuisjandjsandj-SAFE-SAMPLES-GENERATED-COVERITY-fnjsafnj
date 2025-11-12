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

void init_matrix(Matrix* mat) {
    if (mat == NULL) return;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

void input_matrix(Matrix* mat) {
    if (mat == NULL) return;
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int_safe(prompt, INT_MIN, INT_MAX);
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

int add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (result->rows != a->rows || result->cols != a->cols) return 0;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long sum = (long)a->data[i][j] + (long)b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) return 0;
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
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

int transpose_matrix(const Matrix* src, Matrix* result) {
    if (src == NULL || result == NULL) return 0;
    if (result->rows != src->cols || result->cols != src->rows) return 0;
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            result->data[j][i] = src->data[i][j];
        }
    }
    return