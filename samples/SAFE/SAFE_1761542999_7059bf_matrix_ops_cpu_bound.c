//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dim(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_int_safe(const char* prompt, int min_val, int max_val) {
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
        
        if (temp < min_val || temp > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        result->data[i][j] = INT_MAX;
                        break;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        result->data[i][j] = INT_MIN;
                        break;
                    }
                }
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void fill_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), 
                    "Enter element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int_safe(prompt, -1000, 1000);
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Multiplication Program\n");
    printf("Maximum dimension: %d\n", MAX_DIM);
    
    printf("\nFirst matrix dimensions:\n");
    a.rows = read_int_safe("Rows: ", 1, MAX_DIM);
    a.cols = read_int_safe("Columns: ", 1, MAX_DIM);
    
    printf("\nSecond matrix dimensions:\n");
    b.rows = read_int_safe("Rows: ", 1, MAX_DIM);
    b.cols = read_int_safe("Columns: ", 1, MAX_DIM);
    
    if (a.cols != b.rows) {
        printf("Error: Matrix dimensions incompatible for multiplication.\n");
        printf("Columns of first matrix (%d) must equal rows of second matrix (%d).\n", 
               a.cols, b.rows);
        return 1;
    }
    
    if (!validate_matrix_dim(a.rows, a.cols) || 
        !validate_matrix_dim(b.rows, b.cols)) {
        printf("Error: Invalid matrix dimensions.\n");
        return 1;
    }
    
    result.rows = a.rows;
    result.cols = b.cols;
    
    printf("\nEnter elements for first matrix:\n");
    fill_matrix(&a);
    
    printf("\nEnter elements for second matrix:\n");
    fill_matrix(&b);
    
    printf("\nFirst matrix:\n");
    print_matrix(&a);
    
    printf("\nSecond matrix:\n");
    print_matrix(&b);
    
    printf("\nCalculating product...\n");
    matrix_multiply(&a, &b, &result);
    
    printf("\nResult matrix:\n");
    print_matrix(&result);
    
    return 0;
}