//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_ROWS][MAX_COLS];
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
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        break;
    }
    
    return value;
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix dimensions:\n");
    mat->rows = read_int("Rows (1-10): ", 1, MAX_ROWS);
    mat->cols = read_int("Columns (1-10): ", 1, MAX_COLS);
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
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
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * (long long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    return 0;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    
    return 1;
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < dest->rows; i++) {
        for (int j = 0; j < dest->cols; j++) {
            dest->data[i][j] = src->data[j][i];
        }
    }
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    while (1) {
        printf("\nMenu:\n");
        printf("1. Enter matrices\n");
        printf("2. Matrix multiplication\n");
        printf("3. Matrix transpose\n");
        printf("4. Exit\n");
        
        choice = read_int("Choose operation: ", 1, 4);
        if (choice == -1) break;
        
        switch (choice) {
            case 1:
                printf("\nEnter first matrix:\n");
                read_matrix(&a);
                printf("\nEnter second matrix:\n");
                read_matrix(&b);
                printf("\nFirst matrix:\n");
                print_matrix(&a);
                printf("\nSecond matrix:\n");
                print_matrix(&b);
                break;
                
            case 2:
                if (!matrix_multiply(&a, &b, &result)) {
                    printf("Matrix multiplication failed: dimension mismatch or integer overflow.\n");
                } else {
                    printf("\nMultiplication result:\n");
                    print_matrix(&result);
                }
                break;
                
            case 3:
                printf("\nTranspose of first matrix:\n");
                matrix_transpose(&a, &result);
                print_matrix(&result);
                
                printf("\nTranspose of second matrix:\n");
                matrix_transpose(&b, &result);
                print_matrix(&result);
                break;
                
            case 4:
                return 0;
        }
    }
    
    return 0;
}