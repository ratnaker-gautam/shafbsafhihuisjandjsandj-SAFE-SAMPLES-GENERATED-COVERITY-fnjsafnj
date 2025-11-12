//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MATRIX_SIZE (MAX_ROWS * MAX_COLS)

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

#define MATRIX_INIT(m, r, c) do { \
    (m).rows = (r); \
    (m).cols = (c); \
    memset((m).data, 0, sizeof((m).data)); \
} while(0)

#define MATRIX_GET(m, i, j) ((m).data[(i)][(j)])
#define MATRIX_SET(m, i, j, val) ((m).data[(i)][(j)] = (val))

#define MATRIX_ADD(res, a, b) do { \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (a).cols; j++) { \
            (res).data[i][j] = (a).data[i][j] + (b).data[i][j]; \
        } \
    } \
    (res).rows = (a).rows; \
    (res).cols = (a).cols; \
} while(0)

#define MATRIX_MULTIPLY(res, a, b) do { \
    for (int i = 0; i < (a).rows; i++) { \
        for (int j = 0; j < (b).cols; j++) { \
            int sum = 0; \
            for (int k = 0; k < (a).cols; k++) { \
                sum += (a).data[i][k] * (b).data[k][j]; \
            } \
            (res).data[i][j] = sum; \
        } \
    } \
    (res).rows = (a).rows; \
    (res).cols = (b).cols; \
} while(0)

#define MATRIX_PRINT(m) do { \
    for (int i = 0; i < (m).rows; i++) { \
        for (int j = 0; j < (m).cols; j++) { \
            printf("%4d", (m).data[i][j]); \
        } \
        printf("\n"); \
    } \
} while(0)

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return (int)value;
    }
}

void read_matrix(Matrix* m) {
    printf("Enter matrix dimensions:\n");
    m->rows = read_int("Rows (1-10): ", 1, MAX_ROWS);
    m->cols = read_int("Columns (1-10): ", 1, MAX_COLS);
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            MATRIX_SET(*m, i, j, read_int(prompt, INT_MIN, INT_MAX));
        }
    }
}

int main() {
    Matrix a, b, result;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Matrix A:\n");
    read_matrix(&a);
    
    printf("Matrix B:\n");
    read_matrix(&b);
    
    printf("\nAvailable operations:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    operation = read_int("Select operation (1-2): ", 1, 2);
    
    MATRIX_INIT(result, 0, 0);
    
    switch (operation) {
        case 1:
            if (a.rows != b.rows || a.cols != b.cols) {
                printf("Error: Matrices must have same dimensions for addition.\n");
                return 1;
            }
            MATRIX_ADD(result, a, b);
            printf("\nResult of A + B:\n");
            break;
        case 2:
            if (a.cols != b.rows) {
                printf("Error: A columns must equal B rows for multiplication.\n");
                return 1;
            }
            MATRIX_MULTIPLY(result, a, b);
            printf("\nResult of A * B:\n");
            break;
    }
    
    MATRIX_PRINT(result);
    
    return 0;
}