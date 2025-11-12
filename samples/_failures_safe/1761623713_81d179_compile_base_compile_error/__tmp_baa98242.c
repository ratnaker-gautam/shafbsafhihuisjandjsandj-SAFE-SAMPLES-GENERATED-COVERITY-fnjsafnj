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

int read_int(const char* prompt, int min_val, int max_val) {
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
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return value;
    }
}

void read_matrix(struct Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
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

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return a + b;
}

int safe_multiply(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return a * b;
}

void matrix_add(const struct Matrix* a, const struct Matrix* b, struct Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = safe_add(a->data[i][j], b->data[i][j]);
        }
    }
}

void matrix_subtract(const struct Matrix* a, const struct Matrix* b, struct Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = safe_add(a->data[i][j], -b->data[i][j]);
        }
    }
}

void matrix_multiply(const struct Matrix* a, const struct Matrix* b, struct Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int product = safe_multiply(a->data[i][k], b->data[k][j]);
                if ((product > 0 && sum > INT_MAX - product) || 
                    (product < 0 && sum < INT_MIN - product)) {
                    sum = 0;
                    break;
                }
                sum += product;
            }
            result->data[i][j] = sum;
        }
    }
}

void matrix_transpose(const struct Matrix* a, struct Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[j][i] = a->data[i][j];
        }
    }
}

enum Operation get_operation() {
    printf("\nAvailable operations:\n");
    printf("0 - Add matrices\n");
    printf("1 - Subtract matrices\n");
    printf("2 - Multiply matrices\n");
    printf("3 - Transpose matrix\n");
    
    int choice = read_int("Select operation: ", 0, 3);
    return (enum Operation)choice;
}

int main() {
    struct Matrix mat1, mat2, result;
    enum Operation op;
    
    op = get_operation();
    
    switch (op) {
        case OP_ADD:
        case OP_SUBTRACT:
            printf("\nMatrix 1:\n");
            mat1.rows = read_int("Enter rows (1-10): ", 1, 10);
            mat1.cols = read_int("Enter columns (1-10): ", 1, 10);
            read_matrix(&mat1);
            printf