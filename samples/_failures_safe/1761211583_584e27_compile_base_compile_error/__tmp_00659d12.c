//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return value;
    }
}

void init_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int("", INT_MIN, INT_MAX);
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

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return a + b;
}

int safe_multiply(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return a * b;
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = safe_add(a->data[i][j], b->data[i][j]);
        }
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int product = safe_multiply(a->data[i][k], b->data[k][j]);
                if (product == 0 && (a->data[i][k] != 0 && b->data[k][j] != 0)) {
                    sum = 0;
                    break;
                }
                sum = safe_add(sum, product);
                if (sum == 0 && product != 0) {
                    break;
                }
            }
            result->data[i][j] = sum;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    printf("Enter dimensions for first matrix:\n");
    mat1.rows = read_int("Rows: ", MIN_DIM, MAX_ROWS);
    mat1.cols = read_int("Columns: ", MIN_DIM, MAX_COLS);
    
    printf("Enter dimensions for second matrix:\n");
    mat2.rows = read_int("Rows: ", MIN_DIM, MAX_ROWS);
    mat2.cols = read_int("Columns: ", MIN_DIM, MAX_COLS);
    
    printf("\nEnter elements for first matrix:\n");
    init_matrix(&mat1);
    
    printf("\nEnter elements for second matrix:\n");
    init_matrix(&mat2);
    
    while (1) {
        printf("\nChoose operation:\n");
        printf("1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Exit\n");
        
        choice = read_int("Enter choice: ", 1, 3);
        
        if (choice == 3) {
            break;
        }
        
        switch (choice) {
            case 1:
                if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
                    printf("Error: Matrices must have same dimensions for addition.\n");
                    break;
                }
                result.rows = mat1.rows;
                result.cols = mat1.cols;
                add_matrices(&mat1, &mat2, &result);
                printf("\nResult of addition:\n");
                print_matrix(&result);
                break;
                
            case 2:
                if (mat1.cols !=