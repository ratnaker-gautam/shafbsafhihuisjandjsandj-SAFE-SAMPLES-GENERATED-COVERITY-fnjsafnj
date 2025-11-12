//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
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
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d.\n", min, max);
    }
}

void init_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *(*(mat->data + i) + j) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(*(mat->data + i) + j));
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((*(*(a->data + i) + k) > 0 && *(*(b->data + k) + j) > 0 && 
                     *(*(a->data + i) + k) > INT_MAX - sum) ||
                    (*(*(a->data + i) + k) < 0 && *(*(b->data + k) + j) < 0 && 
                     *(*(a->data + i) + k) < INT_MIN - sum)) {
                    return 0;
                }
                sum += *(*(a->data + i) + k) * *(*(b->data + k) + j);
            }
            *(*(result->data + i) + j) = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix* src, Matrix* dst) {
    if (src->rows > MAX_DIM || src->cols > MAX_DIM) {
        return 0;
    }
    
    dst->rows = src->cols;
    dst->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(*(dst->data + j) + i) = *(*(src->data + i) + j);
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations using Pointer Arithmetic\n");
    printf("==========================================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    init_matrix(&mat1);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nChoose operation:\n");
    printf("1. Matrix multiplication\n");
    printf("2. Matrix transpose\n");
    
    int choice = read_int("Enter choice (1-2): ", 1, 2);
    
    if (choice == 1) {
        printf("\nMatrix 2 dimensions must be %d x N\n", mat1.cols);
        mat2.rows = mat1.cols;
        mat2.cols = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
        init_matrix(&mat2);
        
        printf("\nMatrix 2:\n");
        print_matrix(&mat2);
        
        if (multiply_matrices(&mat1, &mat2, &result)) {
            printf("\nMatrix multiplication result:\n");
            print_matrix(&result);
        } else {
            printf("Matrix multiplication failed: dimension mismatch or integer overflow.\n");
        }
    } else {
        if (transpose_matrix(&mat1, &result)) {
            printf("\nMatrix transpose result:\n");
            print_matrix(&result);
        } else {
            printf("Matrix transpose failed.\n");
        }
    }
    
    return 0;
}