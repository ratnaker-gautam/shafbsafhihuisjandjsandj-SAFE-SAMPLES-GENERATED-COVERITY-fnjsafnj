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
            return -1;
        }
        
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        break;
    }
    
    return value;
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    result->rows = a->rows;
    result->cols = a->cols;
    
    int i = 0;
    while (i < a->rows) {
        int j = 0;
        do {
            if (a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) {
                printf("Integer overflow detected!\n");
                exit(1);
            }
            if (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j]) {
                printf("Integer overflow detected!\n");
                exit(1);
            }
            result->data[i][j] = a->data[i][j] + b->data[i][j];
            j++;
        } while (j < a->cols);
        i++;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            int k = 0;
            while (k < a->cols) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if (a->data[i][k] != 0 && product / a->data[i][k] != b->data[k][j]) {
                    printf("Integer overflow detected!\n");
                    exit(1);
                }
                if (sum > 0 && product > LLONG_MAX - sum) {
                    printf("Integer overflow detected!\n");
                    exit(1);
                }
                if (sum < 0 && product < LLONG_MIN - sum) {
                    printf("Integer overflow detected!\n");
                    exit(1);
                }
                sum += product;
                k++;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                printf("Integer overflow detected!\n");
                exit(1);
            }
            result->data[i][j] = (int)sum;
        }
    }
}

void transpose_matrix(const Matrix* mat, Matrix* result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    int i = 0;
    do {
        int j = 0;
        while (j < mat->rows) {
            result->data[i][j] = mat->data[j][i];
            j++;
        }
        i++;
    } while (i < mat->cols);
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    mat1.cols = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    read_matrix(&mat1);
    
    mat2.rows = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_SIZE);
    mat2.cols = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_SIZE);
    read_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    printf("\nMatrix 2:\n");
    print_matrix(&mat