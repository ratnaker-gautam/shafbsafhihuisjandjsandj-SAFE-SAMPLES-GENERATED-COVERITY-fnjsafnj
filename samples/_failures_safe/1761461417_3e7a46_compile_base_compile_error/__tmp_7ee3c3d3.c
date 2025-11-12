//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10
#define MATRIX_ELEMENT_MAX 1000
#define MATRIX_ELEMENT_MIN -1000

#define MATRIX_OP_ADD 1
#define MATRIX_OP_SUBTRACT 2
#define MATRIX_OP_MULTIPLY 3

#define SAFE_MALLOC(size, ptr) do { \
    ptr = malloc(size); \
    if (!ptr) { \
        fprintf(stderr, "Memory allocation failed\n"); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

#define VALIDATE_SIZE(n) do { \
    if (n <= 0 || n > MAX_SIZE) { \
        fprintf(stderr, "Invalid matrix size\n"); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

#define VALIDATE_ELEMENT(val) do { \
    if (val < MATRIX_ELEMENT_MIN || val > MATRIX_ELEMENT_MAX) { \
        fprintf(stderr, "Matrix element out of range\n"); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

void read_matrix(Matrix *mat) {
    char buffer[256];
    int rows, cols;
    
    printf("Enter number of rows: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        fprintf(stderr, "Input error\n");
        exit(EXIT_FAILURE);
    }
    if (sscanf(buffer, "%d", &rows) != 1) {
        fprintf(stderr, "Invalid input for rows\n");
        exit(EXIT_FAILURE);
    }
    VALIDATE_SIZE(rows);
    
    printf("Enter number of columns: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        fprintf(stderr, "Input error\n");
        exit(EXIT_FAILURE);
    }
    if (sscanf(buffer, "%d", &cols) != 1) {
        fprintf(stderr, "Invalid input for columns\n");
        exit(EXIT_FAILURE);
    }
    VALIDATE_SIZE(cols);
    
    mat->rows = rows;
    mat->cols = cols;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (!fgets(buffer, sizeof(buffer), stdin)) {
                fprintf(stderr, "Input error\n");
                exit(EXIT_FAILURE);
            }
            int value;
            if (sscanf(buffer, "%d", &value) != 1) {
                fprintf(stderr, "Invalid matrix element\n");
                exit(EXIT_FAILURE);
            }
            VALIDATE_ELEMENT(value);
            mat->data[i][j] = value;
        }
    }
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long long sum = (long long)a->data[i][j] + b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) {
                return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int matrix_subtract(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long long diff = (long long)a->data[i][j] - b->data[i][j];
            if (diff > INT_MAX || diff < INT_MIN) {
                return 0;
            }
            result->data[i][j] = (int)diff;
        }
    }
    return 1;
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long