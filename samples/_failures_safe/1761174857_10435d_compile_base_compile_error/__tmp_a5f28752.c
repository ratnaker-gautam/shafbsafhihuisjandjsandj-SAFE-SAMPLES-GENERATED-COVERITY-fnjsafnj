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
            continue;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void init_matrix(Matrix* mat, int rows, int cols) {
    if (rows <= 0 || rows > MAX_DIM || cols <= 0 || cols > MAX_DIM) {
        return;
    }
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void input_matrix(Matrix* mat) {
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
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(mat->data + i * mat->cols + j));
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) {
        return 0;
    }
    if (a->cols != b->rows) {
        return 0;
    }
    
    if (a->rows > MAX_DIM || b->cols > MAX_DIM) {
        return 0;
    }
    
    init_matrix(result, a->rows, b->cols);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int term1 = *(a->data + i * a->cols + k);
                int term2 = *(b->data + k * b->cols + j);
                if ((term1 > 0 && term2 > 0 && term1 > INT_MAX / term2) ||
                    (term1 > 0 && term2 < 0 && term2 < INT_MIN / term1) ||
                    (term1 < 0 && term2 > 0 && term1 < INT_MIN / term2) ||
                    (term1 < 0 && term2 < 0 && term1 < INT_MAX / term2)) {
                    return 0;
                }
                int product = term1 * term2;
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                sum += product;
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
    
    return 1;
}

int transpose_matrix(const Matrix* src, Matrix* dst) {
    if (src == NULL || dst == NULL) {
        return 0;
    }
    if (src->rows > MAX_DIM || src->cols > MAX_DIM) {
        return 0;
    }
    
    init_matrix(dst, src->cols, src->rows);
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dst->data + j * dst->cols + i) = *(src->data + i * src->cols + j);
        }
    }
    
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations using Pointer Arithmetic\n");
    printf("==========================================\n");
    
    while (1) {
        printf("\nOperations:\n");
        printf("1. Matrix Multiplication\n");
        printf("2. Matrix Transpose\n");
        printf("3. Exit\n");
        
        choice = read_int("Choose operation (1-3): ", 1, 3);
        
        if (choice == 3) {
            break;
        }
        
        if (choice == 1) {
            printf("\nMatrix A:\n");
            int rows1 = read_int("Enter