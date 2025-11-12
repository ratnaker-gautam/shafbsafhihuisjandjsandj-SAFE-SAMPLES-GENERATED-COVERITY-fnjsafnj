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
        if (fgets(input, sizeof(input), stdin) == NULL) return min_val;
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
        else if (len == sizeof(input)-1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        if (safe_str_to_int(input, &value) && value >= min_val && value <= max_val) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min_val, max_val);
    }
}

void matrix_init(Matrix* m) {
    if (m == NULL) return;
    m->rows = 0;
    m->cols = 0;
    memset(m->data, 0, sizeof(m->data));
}

void matrix_read(Matrix* m) {
    if (m == NULL) return;
    printf("Enter matrix dimensions:\n");
    m->rows = read_int_safe("Rows: ", MIN_DIM, MAX_ROWS);
    m->cols = read_int_safe("Cols: ", MIN_DIM, MAX_COLS);
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            m->data[i][j] = read_int_safe(prompt, INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* m) {
    if (m == NULL) return;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

void matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) return;
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrix dimensions don't match for addition.\n");
        return;
    }
    result->rows = a->rows;
    result->cols = a->cols;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long sum = (long)a->data[i][j] + (long)b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) {
                printf("Integer overflow in addition.\n");
                return;
            }
            result->data[i][j] = (int)sum;
        }
    }
}

void matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) return;
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return;
    }
    result->rows = a->rows;
    result->cols = b->cols;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow in multiplication.\n");
                    return;
                }
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow in multiplication.\n");
                    return;
                }
            }
            result->data[i][j] = (int)sum;
        }
    }
}

void matrix_transpose(const Matrix* m, Matrix* result) {
    if (m == NULL || result == NULL) return;
    result->rows = m->cols;
    result->cols = m->rows;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->