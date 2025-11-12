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
                } else if (a->data[i][k] < 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) {
                        result->data[i][j] = INT_MIN;
                        break;
                    }
                } else if (a->data[i][k] > 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        result->data[i][j] = INT_MAX;
                        break;
                    }
                }
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void print_matrix(const Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8d", m->data[i][j]);
        }
        printf("\n");
    }
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

void fill_matrix(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            m->data[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Multiplication Program\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    printf("First matrix dimensions:\n");
    a.rows = read_int("Rows: ", 1, MAX_DIM);
    a.cols = read_int("Columns: ", 1, MAX_DIM);
    
    printf("\nSecond matrix dimensions:\n");
    b.rows = read_int("Rows: ", 1, MAX_DIM);
    b.cols = read_int("Columns: ", 1, MAX_DIM);
    
    if (a.cols != b.rows) {
        printf("Error: Matrix dimensions incompatible for multiplication.\n");
        printf("Columns of first matrix (%d) must equal rows of second matrix (%d).\n", a.cols, b.rows);
        return 1;
    }
    
    if (!validate_matrix_dim(a.rows, a.cols) || !validate_matrix_dim(b.rows, b.cols)) {
        printf("Error: Invalid matrix dimensions.\n");
        return 1;
    }
    
    result.rows = a.rows;
    result.cols = b.cols;
    
    printf("\nEnter values for first matrix:\n");
    fill_matrix(&a);
    
    printf("\nEnter values for second matrix:\n");
    fill_matrix(&b);
    
    printf("\nMultiplying matrices...\n");
    matrix_multiply(&a, &b, &result);
    
    printf("\nFirst matrix:\n");
    print_matrix(&a);
    
    printf("\nSecond matrix:\n");
    print_matrix(&b);
    
    printf("\nResult matrix:\n");
    print_matrix(&result);
    
    return 0;
}