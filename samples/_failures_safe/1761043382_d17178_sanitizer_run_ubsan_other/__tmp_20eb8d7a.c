//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Enter value between %d and %d.\n", min_val, max_val);
    }
}

void read_matrix(Matrix* mat) {
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
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((a->data[i][k] > 0 && b->data[k][j] > 0 && sum > INT_MAX - a->data[i][k] * b->data[k][j]) ||
                    (a->data[i][k] < 0 && b->data[k][j] < 0 && sum < INT_MIN - a->data[i][k] * b->data[k][j]) ||
                    (a->data[i][k] > 0 && b->data[k][j] < 0 && sum < INT_MIN + a->data[i][k] * b->data[k][j]) ||
                    (a->data[i][k] < 0 && b->data[k][j] > 0 && sum < INT_MIN + a->data[i][k] * b->data[k][j])) {
                    return 0;
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    read_matrix(&mat1);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    while (1) {
        printf("\nOperations:\n");
        printf("1. Multiply with another matrix\n");
        printf("2. Transpose\n");
        printf("3. Exit\n");
        
        choice = read_int("Choose operation: ", 1, 3);
        
        if (choice == 1) {
            mat2.rows = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
            mat2.cols = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
            read_matrix(&mat2);
            
            printf("\nMatrix 2:\n");
            print_matrix(&mat2);
            
            if (matrix_multiply(&mat1, &mat2, &result)) {
                printf("\nMultiplication result:\n");
                print_matrix(&result);
            } else {
                printf("Matrix multiplication not possible or overflow detected.\n");
            }
        } else if (choice == 2) {
            matrix_transpose(&mat1, &result);
            printf("\nTranspose of matrix 1:\n");
            print_matrix(&result);
        } else {
            break;
        }
    }
    
    return 0;
}