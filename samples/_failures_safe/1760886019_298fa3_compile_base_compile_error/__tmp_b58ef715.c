//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
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

void matrix_init(Matrix* mat, int rows, int cols) {
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat, const char* name) {
    printf("Enter values for matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "  [%d][%d]: ", i, j);
            *(mat->data + i * mat->cols + j) = read_int(prompt, -100, 100);
        }
    }
}

void matrix_print(const Matrix* mat, const char* name) {
    printf("Matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        printf("  ");
        for (int j = 0; j < mat->cols; j++) {
            printf("%4d ", *(mat->data + i * mat->cols + j));
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    if (result->rows != a->rows || result->cols != b->cols) {
        return 0;
    }
    
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
                if ((sum > 0 && product > INT_MAX - sum) ||
                    (sum < 0 && product < INT_MIN - sum)) {
                    return 0;
                }
                sum += product;
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
    
    return 1;
}

int matrix_transpose(const Matrix* src, Matrix* dest) {
    if (src->rows != dest->cols || src->cols != dest->rows) {
        return 0;
    }
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest->data + j * dest->cols + i) = *(src->data + i * src->cols + j);
        }
    }
    
    return 1;
}

int main() {
    Matrix a, b, result, transposed;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    printf("Enter dimensions for matrix A:\n");
    int rows_a = read_int("  Rows (1-16): ", 1, MAX_DIM);
    int cols_a = read_int("  Columns (1-16): ", 1, MAX_DIM);
    matrix_init(&a, rows_a, cols_a);
    matrix_input(&a, "A");
    
    printf("\nEnter dimensions for matrix B:\n");
    int rows_b = read_int("  Rows (1-16): ", 1, MAX_DIM);
    int cols_b = read_int("  Columns (1-16): ", 1, MAX_DIM);
    matrix_init(&b, rows_b, cols_b);
    matrix_input(&b, "B");
    
    printf("\n");
    matrix_print(&a, "A");
    printf("\n");
    matrix_print(&b, "B");
    
    while (1) {
        printf