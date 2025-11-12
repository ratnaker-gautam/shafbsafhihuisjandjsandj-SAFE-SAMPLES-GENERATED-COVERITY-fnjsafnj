//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM * MAX_DIM];
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
        long val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (val < min || val > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)val;
        break;
    }
    
    return value;
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix dimensions:\n");
    mat->rows = read_int("Rows: ", 1, MAX_DIM);
    mat->cols = read_int("Columns: ", 1, MAX_DIM);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *(mat->data + i * mat->cols + j) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(mat->data + i * mat->cols + j));
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrix dimensions don't match for addition.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int idx = i * a->cols + j;
            int sum = *(a->data + idx) + *(b->data + idx);
            if ((*(a->data + idx) > 0 && *(b->data + idx) > 0 && sum < 0) ||
                (*(a->data + idx) < 0 && *(b->data + idx) < 0 && sum > 0)) {
                printf("Integer overflow detected in addition.\n");
                return;
            }
            *(result->data + idx) = sum;
        }
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
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
                long product = (long)*(a->data + i * a->cols + k) * 
                              (long)*(b->data + k * b->cols + j);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication.\n");
                    return;
                }
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
            }
            *(result->data + i * b->cols + j) = (int)sum;
        }
    }
}

void transpose_matrix(const Matrix* mat, Matrix* result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            *(result->data + j * mat->rows + i) = *(mat->data + i * mat->cols + j);
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Enter first matrix:\n");
    read_matrix(&mat1);
    
    printf("Enter second matrix:\n");
    read_matrix(&mat2);
    
    while (1) {
        printf("\nAvailable operations:\n");
        printf("1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Transpose first matrix\n");
        printf("4. Transpose second matrix\n");
        printf("5. Exit\n");