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

int read_int(const char* prompt) {
    char input[MAX_INPUT_LEN + 1];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
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
        if (safe_str_to_int(input, &value)) {
            return value;
        }
        printf("Invalid integer\n");
    }
}

void init_matrix(Matrix* mat) {
    if (mat == NULL) return;
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            mat->data[i][j] = 0;
        }
    }
    mat->rows = 0;
    mat->cols = 0;
}

int input_matrix(Matrix* mat) {
    if (mat == NULL) return 0;
    printf("Enter number of rows (1-%d): ", MAX_ROWS);
    mat->rows = read_int("");
    if (mat->rows < MIN_DIM || mat->rows > MAX_ROWS) return 0;
    printf("Enter number of columns (1-%d): ", MAX_COLS);
    mat->cols = read_int("");
    if (mat->cols < MIN_DIM || mat->cols > MAX_COLS) return 0;
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int("");
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

int add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    if (a->rows > MAX_ROWS || a->cols > MAX_COLS) return 0;
    result->rows = a->rows;
    result->cols = a->cols;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                return 0;
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->cols != b->rows) return 0;
    if (a->rows > MAX_ROWS || b->cols > MAX_COLS) return 0;
    result->rows = a->rows;
    result->cols = b->cols;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > 0 && sum > LONG_MAX - product) return 0;
                if (product < 0 && sum < LONG_MIN - product) return 0;
                sum += product;
            }