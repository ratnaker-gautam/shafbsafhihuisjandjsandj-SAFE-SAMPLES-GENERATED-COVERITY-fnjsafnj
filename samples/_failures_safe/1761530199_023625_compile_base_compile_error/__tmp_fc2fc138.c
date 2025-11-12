//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 16

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
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter value between %d and %d.\n", min, max);
    }
}

void matrix_init(Matrix* mat, int rows, int cols) {
    if (rows <= 0 || rows > MAX_DIM || cols <= 0 || cols > MAX_DIM) {
        return;
    }
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat, const char* name) {
    if (mat == NULL || name == NULL) return;
    printf("Enter values for matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "  [%d,%d]: ", i, j);
            *(ptr + i * mat->cols + j) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* mat, const char* name) {
    if (mat == NULL || name == NULL) return;
    printf("Matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    const int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        printf("  ");
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(ptr + i * mat->cols + j));
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->cols != b->rows) {
        return 0;
    }
    
    matrix_init(result, a->rows, b->cols);
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* r_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int term1 = *(a_ptr + i * a->cols + k);
                int term2 = *(b_ptr + k * b->cols + j);
                if ((term1 > 0 && term2 > 0 && term1 > INT_MAX / term2) ||
                    (term1 > 0 && term2 < 0 && term2 < INT_MIN / term1) ||
                    (term1 < 0 && term2 > 0 && term1 < INT_MIN / term2) ||
                    (term1 < 0 && term2 < 0 && term1 < INT_MAX / term2)) {
                    return 0;
                }
                int product = term1 * term2;
                if ((term1 > 0 && term2 > 0 && sum > INT_MAX - product) ||
                    (term1 < 0 && term2 < 0 && sum < INT_MIN - product) ||
                    (term1 > 0 && term2 < 0 && sum < INT_MIN - product) ||
                    (term1 < 0 && term2 > 0 && sum > INT_MAX - product)) {
                    return 0;
                }
                sum += product;
            }
            *(r_ptr + i * result->cols + j) = sum;
        }
    }
    return 1;
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
    if (src == NULL || dest == NULL) return;
    matrix_init(dest, src->cols, src->rows);
    const int* src_ptr = src->data;
    int* dest_ptr = dest->data;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest_ptr + j * dest->cols + i) = *(src_ptr + i * src->cols + j);
        }
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    int rows1 = read_int("Enter rows for matrix A (1-16): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix A (1-16): ", 1, MAX_D