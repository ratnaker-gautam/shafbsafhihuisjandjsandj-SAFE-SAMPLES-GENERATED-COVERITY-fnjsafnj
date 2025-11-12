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
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter value between %d and %d.\n", min, max);
    }
}

void matrix_init(Matrix* mat) {
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat) {
    mat->rows = read_int("Enter rows (1-16): ", 1, MAX_DIM);
    mat->cols = read_int("Enter cols (1-16): ", 1, MAX_DIM);
    
    printf("Enter matrix elements:\n");
    int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            *ptr = read_int("", INT_MIN, INT_MAX);
            ptr++;
        }
    }
}

void matrix_print(const Matrix* mat) {
    const int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *ptr);
            ptr++;
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    matrix_init(result);
    
    int* res_ptr = result->data;
    const int* a_ptr = a->data;
    
    for (int i = 0; i < a->rows; i++) {
        const int* b_ptr = b->data;
        for (int j = 0; j < b->cols; j++) {
            const int* a_row_ptr = a_ptr;
            const int* b_col_ptr = b_ptr;
            int sum = 0;
            
            for (int k = 0; k < a->cols; k++) {
                if ((*a_row_ptr > 0 && *b_col_ptr > 0 && *a_row_ptr > INT_MAX / *b_col_ptr) ||
                    (*a_row_ptr < 0 && *b_col_ptr < 0 && *a_row_ptr < INT_MAX / *b_col_ptr) ||
                    (*a_row_ptr > 0 && *b_col_ptr < 0 && *b_col_ptr < INT_MIN / *a_row_ptr) ||
                    (*a_row_ptr < 0 && *b_col_ptr > 0 && *a_row_ptr < INT_MIN / *b_col_ptr)) {
                    return 0;
                }
                int product = *a_row_ptr * *b_col_ptr;
                if ((*a_row_ptr > 0 && *b_col_ptr > 0 && product < 0) ||
                    (*a_row_ptr < 0 && *b_col_ptr < 0 && product < 0) ||
                    (*a_row_ptr > 0 && *b_col_ptr < 0 && product > 0) ||
                    (*a_row_ptr < 0 && *b_col_ptr > 0 && product > 0)) {
                    return 0;
                }
                if (sum > 0 && product > INT_MAX - sum) return 0;
                if (sum < 0 && product < INT_MIN - sum) return 0;
                sum += product;
                a_row_ptr++;
                b_col_ptr += b->cols;
            }
            *res_ptr = sum;
            res_ptr++;
            b_ptr++;
        }
        a_ptr += a->cols;
    }
    return 1;
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    const int* src_ptr = src->data;
    int* dest_ptr = dest->data;
    
    for (int i = 0; i < src->cols; i++) {
        const int* col_ptr = src_ptr + i;
        for (int j = 0; j < src->rows; j++) {
            *dest_ptr = *col_ptr;
            dest_ptr++;
            col_ptr += src->cols;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Input matrices\n");
    printf("2. Multiply matrices\n");
    printf("3. Transpose matrix\n");
    printf("4. Exit\n");
    
    matrix_init(&mat1);
    matrix_init(&mat2);
    matrix_init(&result);
    
    while (1) {
        choice = read_int("Enter choice (1-4): ", 1, 4);
        
        switch (choice) {