//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[32];
    
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

void read_matrix(Matrix* mat) {
    int i = 0;
    int j = 0;
    
    printf("Enter matrix elements:\n");
    while (i < mat->rows) {
        j = 0;
        while (j < mat->cols) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, INT_MIN, INT_MAX);
            j++;
        }
        i++;
    }
}

void print_matrix(const Matrix* mat) {
    int i = 0;
    do {
        int j = 0;
        while (j < mat->cols) {
            printf("%6d", mat->data[i][j]);
            j++;
        }
        printf("\n");
        i++;
    } while (i < mat->rows);
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    int i, j, k;
    
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < b->cols; j++) {
            result->data[i][j] = 0;
            k = 0;
            while (k < a->cols) {
                if ((a->data[i][k] > 0 && b->data[k][j] > 0 && a->data[i][k] > INT_MAX / b->data[k][j]) ||
                    (a->data[i][k] < 0 && b->data[k][j] < 0 && a->data[i][k] < INT_MAX / b->data[k][j]) ||
                    (a->data[i][k] > 0 && b->data[k][j] < 0 && a->data[i][k] > INT_MIN / b->data[k][j]) ||
                    (a->data[i][k] < 0 && b->data[k][j] > 0 && a->data[i][k] < INT_MIN / b->data[k][j])) {
                    printf("Integer overflow detected in multiplication.\n");
                    result->data[i][j] = 0;
                    break;
                }
                result->data[i][j] += a->data[i][k] * b->data[k][j];
                k++;
            }
        }
    }
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    int i = 0;
    int j;
    
    do {
        j = 0;
        while (j < src->cols) {
            dest->data[j][i] = src->data[i][j];
            j++;
        }
        i++;
    } while (i < src->rows);
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-8): ", 1, MAX_SIZE);
    mat1.cols = read_int("Enter columns for matrix 1 (1-8): ", 1, MAX_SIZE);
    read_matrix(&mat1);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    while (1) {
        printf("\nOperations:\n");
        printf("1. Multiply with another matrix\n");
        printf("2. Transpose\n");
        printf("3. Exit\n");
        
        choice = read_int("Choose operation: ", 1, 3);
        
        switch (choice) {
            case 1: {
                mat2.rows = mat1.cols;
                mat2.cols = read_int("Enter columns for matrix 2 (1-8): ", 1, MAX_SIZE);
                read_matrix(&mat2);
                
                result.rows = mat1.rows;
                result.cols = mat2.cols;
                
                multiply_matrices(&mat1, &mat2, &result);
                
                printf("\nMatrix 2:\n");
                print_matrix(&mat2);
                printf("\nResult of multiplication:\n");
                print_matrix(&result);
                break;
            }
            case 2: {
                result.rows = mat1.cols;
                result.cols = mat1.rows;
                transpose_matrix(&mat1, &result);
                
                printf("\nTranspose of matrix 1:\n");
                print_matrix(&result);
                break;
            }
            case 3:
                return 0;
        }
    }
    
    return 0;
}