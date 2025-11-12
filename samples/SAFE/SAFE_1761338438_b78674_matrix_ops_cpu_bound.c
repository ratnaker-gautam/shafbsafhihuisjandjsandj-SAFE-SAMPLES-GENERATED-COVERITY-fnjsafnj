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

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
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
                } else if (a->data[i][k] != 0 && b->data[k][j] != 0) {
                    int64_t product = (int64_t)a->data[i][k] * (int64_t)b->data[k][j];
                    if (product > INT_MAX) {
                        result->data[i][j] = INT_MAX;
                        break;
                    } else if (product < INT_MIN) {
                        result->data[i][j] = INT_MIN;
                        break;
                    }
                }
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d", m->data[i][j]);
            if (j < m->cols - 1) printf(" ");
        }
        printf("\n");
    }
}

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
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

void read_matrix(Matrix *m) {
    m->rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    if (m->rows == -1) exit(1);
    
    m->cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    if (m->cols == -1) exit(1);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            m->data[i][j] = read_int(prompt, -1000, 1000);
            if (m->data[i][j] == -1) exit(1);
        }
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix A:\n");
    read_matrix(&a);
    
    printf("Matrix B:\n");
    read_matrix(&b);
    
    if (a.cols != b.rows) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d) for multiplication.\n", a.cols, b.rows);
        return 1;
    }
    
    if (!validate_matrix_dim(a.rows, b.cols)) {
        printf("Error: Result matrix dimensions would exceed maximum size.\n");
        return 1;
    }
    
    result.rows = a.rows;
    result.cols = b.cols;
    
    matrix_multiply(&a, &b, &result);
    
    printf("\nResult matrix (A × B):\n");
    print_matrix(&result);
    
    return 0;
}