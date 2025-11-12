//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

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
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                printf("Integer overflow detected at [%d][%d].\n", i, j);
                result->data[i][j] = 0;
            } else {
                result->data[i][j] = a->data[i][j] + b->data[i][j];
            }
            j++;
        } while (j < a->cols);
        i++;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    init_matrix(result);
    
    for (int i = 0; i < a->rows; i++) {
        int j = 0;
        while (j < b->cols) {
            int k = 0;
            do {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected at [%d][%d].\n", i, j);
                    result->data[i][j] = 0;
                    break;
                }
                
                long long sum = (long long)result->data[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow detected at [%d][%d].\n", i, j);
                    result->data[i][j] = 0;
                    break;
                }
                
                result->data[i][j] = (int)sum;
                k++;
            } while (k < a->cols);
            j++;
        }
    }
}

void transpose_matrix(const Matrix* mat, Matrix* result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    int i = 0;
    while (i < mat->rows) {
        int j = 0;
        do {
            result->data[j][i] = mat->data[i][j];
            j++;
        } while (j < mat->cols);
        i++;
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    mat1.cols = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    input_matrix(&mat