//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

enum Operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_INVALID
};

struct Matrix {
    int rows;
    int cols;
    int data[10][10];
};

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

void read_matrix(struct Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows (1-10): ", 1, 10);
    mat->cols = read_int("Columns (1-10): ", 1, 10);
    
    printf("Enter %d elements for matrix %s:\n", mat->rows * mat->cols, name);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const struct Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int add_matrices(const struct Matrix* a, const struct Matrix* b, struct Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int subtract_matrices(const struct Matrix* a, const struct Matrix* b, struct Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return 1;
}

int multiply_matrices(const struct Matrix* a, const struct Matrix* b, struct Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return 1;
}

void transpose_matrix(const struct Matrix* a, struct Matrix* result) {
    result->rows = a->cols;
    result->cols = a->rows;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[j][i] = a->data[i][j];
        }
    }
}

enum Operation get_operation() {
    printf("Select operation:\n");
    printf("0 - Add matrices\n");
    printf("1 - Subtract matrices\n");
    printf("2 - Multiply matrices\n");
    printf("3 - Transpose matrix\n");
    
    int choice = read_int("Enter choice (0-3): ", 0, 3);
    return (enum Operation)choice;
}

int main() {
    struct Matrix mat1, mat2, result;
    enum Operation op = get_operation();
    
    switch (op) {
        case OP_ADD:
            read_matrix(&mat1, "A");
            read_matrix(&mat2, "B");
            if (add_matrices(&mat1, &mat2, &result)) {
                printf("Result of A + B:\n");
                print_matrix(&result);
            } else {
                printf("Error: Matrices must have same dimensions for addition.\n");
            }
            break;
            
        case OP_SUBTRACT:
            read_matrix(&mat1, "A");