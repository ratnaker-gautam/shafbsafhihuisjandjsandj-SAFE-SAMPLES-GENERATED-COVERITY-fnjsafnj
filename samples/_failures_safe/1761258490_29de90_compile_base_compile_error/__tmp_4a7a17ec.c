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
    if (val < INT_MIN || val > INT_MAX) return 0;
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
            exit(EXIT_FAILURE);
        }
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        } else if (len == sizeof(input)-1) {
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

void matrix_init(Matrix* mat) {
    if (mat == NULL) return;
    mat->rows = 0;
    mat->cols = 0;
    memset(mat->data, 0, sizeof(mat->data));
}

int matrix_input(Matrix* mat) {
    if (mat == NULL) return 0;
    printf("Enter matrix dimensions (rows cols):\n");
    int rows = read_int("Rows: ");
    int cols = read_int("Cols: ");
    if (rows < MIN_DIM || rows > MAX_ROWS || cols < MIN_DIM || cols > MAX_COLS) {
        printf("Invalid dimensions\n");
        return 0;
    }
    mat->rows = rows;
    mat->cols = cols;
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int("");
        }
    }
    return 1;
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
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((a->data[i][k] > 0 && b->data[k][j] > 0 && a->data[i][k] > INT_MAX / b->data[k][j]) ||
                    (a->data[i][k] < 0 && b->data[k][j] < 0 && a->data[i][k] < INT_MAX / b->data[k][j]) ||
                    (a->data[i][k] > 0 && b->data[k][j] < 0 && a->data[i][k] > INT_MIN / b->data[k][j]) ||
                    (a->data[i][k] < 0 && b->data[k][j] > 0 && a->data[i][k] < INT_MIN / b->data[k][j])) {
                    return 0;
                }
                int product = a->data[i][k] * b->data[k][j];
                if ((product > 0 && sum > INT_MAX - product) || (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                sum += product;
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
        for (int j = 0;