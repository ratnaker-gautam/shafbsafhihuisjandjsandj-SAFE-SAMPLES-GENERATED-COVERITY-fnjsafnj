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
            return -1;
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

void matrix_init(Matrix* mat, int rows, int cols) {
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat) {
    printf("Enter matrix elements:\n");
    int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *ptr = read_int(prompt, INT_MIN, INT_MAX);
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
    
    if (a->rows > MAX_DIM || b->cols > MAX_DIM) {
        return 0;
    }
    
    matrix_init(result, a->rows, b->cols);
    
    int* res_ptr = result->data;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            const int* a_row = a->data + (i * a->cols);
            const int* b_col = b->data + j;
            
            for (int k = 0; k < a->cols; k++) {
                if ((sum > 0 && *a_row > INT_MAX - sum) || 
                    (sum < 0 && *a_row < INT_MIN - sum)) {
                    return 0;
                }
                sum += *a_row * (*b_col);
                a_row++;
                b_col += b->cols;
            }
            
            *res_ptr = sum;
            res_ptr++;
        }
    }
    
    return 1;
}

int matrix_transpose(const Matrix* src, Matrix* dst) {
    if (src->rows > MAX_DIM || src->cols > MAX_DIM) {
        return 0;
    }
    
    matrix_init(dst, src->cols, src->rows);
    
    int* dst_ptr = dst->data;
    for (int j = 0; j < src->cols; j++) {
        const int* src_col = src->data + j;
        for (int i = 0; i < src->rows; i++) {
            *dst_ptr = *src_col;
            dst_ptr++;
            src_col += src->cols;
        }
    }
    
    return 1;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Matrix A dimensions:\n");
    int rows_a = read_int("Rows (1-16): ", 1, MAX_DIM);
    int cols_a = read_int("Columns (1-16): ", 1, MAX_DIM);
    matrix_init(&a, rows_a, cols_a);
    matrix_input(&a);
    
    printf("\nMatrix B dimensions:\n");
    int rows_b = read_int("Rows (1-16): ", 1, MAX_DIM);
    int cols_b = read_int("Columns (1-16): ", 1, MAX_DIM);
    matrix_init(&b, rows_b, cols_b);
    matrix_input(&b);
    
    printf("\nMatrix A:\n");
    matrix_print(&a);
    
    printf("\nMatrix B:\n");
    matrix_print(&b);
    
    printf("\nMatrix A Transpose:\n");
    if (matrix_transpose(&a, &result)) {
        matrix_print(&result);
    } else {
        printf("Transpose failed.\n");
    }
    
    printf("\nMatrix B Transpose:\n");
    if (matrix_transpose(&b, &result)) {
        matrix_print(&result);
    } else {
        printf("Transpose failed.\n");
    }
    
    printf("\nMatrix A * B:\