//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
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

void init_matrix(Matrix* mat) {
    int i = 0;
    while (i < mat->rows) {
        int j = 0;
        do {
            mat->data[i][j] = 0;
            j++;
        } while (j < mat->cols);
        i++;
    }
}

void input_matrix(Matrix* mat) {
    printf("Enter matrix elements (%dx%d):\n", mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    int i = 0;
    while (i < mat->rows) {
        int j = 0;
        do {
            printf("%6d", mat->data[i][j]);
            j++;
        } while (j < mat->cols);
        printf("\n");
        i++;
    }
}

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return a + b;
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrix dimensions don't match for addition.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    int i = 0;
    while (i < a->rows) {
        int j = 0;
        do {
            result->data[i][j] = safe_add(a->data[i][j], b->data[i][j]);
            j++;
        } while (j < a->cols);
        i++;
    }
}

int safe_multiply(int a, int b) {
    if (a == 0 || b == 0) return 0;
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
            if (a < INT_MAX / b) return 0;
        }
    }
    return a * b;
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    init_matrix(result);
    
    int i = 0;
    while (i < a->rows) {
        int j = 0;
        do {
            int k = 0;
            while (k < a->cols) {
                int product = safe_multiply(a->data[i][k], b->data[k][j]);
                if (product != 0) {
                    result->data[i][j] = safe_add(result->data[i][j], product);
                }
                k++;
            }
            j++;
        } while (j < b->cols);
        i++;
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-8): ", 1, MAX_SIZE);
    mat1.cols = read_int("Enter columns for matrix 1 (1-8): ", 1, MAX_SIZE);
    input_matrix(&mat1);
    
    mat2.rows = read_int("Enter rows for matrix 2 (1-8): ", 1, MAX_SIZE);
    mat2.cols = read_int("Enter columns for matrix 2 (1-8): ", 1, MAX_SIZE);
    input_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix Addition:\n");
    add_matrices(&mat1, &