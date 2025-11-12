//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
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
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

void init_matrix(Matrix* mat) {
    printf("Initializing matrix %dx%d:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "  Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, -100, 100);
        }
    }
}

void print_matrix(const Matrix* mat) {
    printf("Matrix %dx%d:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        printf("  ");
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((b->data[i][j] > 0 && a->data[i][j] > INT_MAX - b->data[i][j]) ||
                (b->data[i][j] < 0 && a->data[i][j] < INT_MIN - b->data[i][j])) {
                result->data[i][j] = (a->data[i][j] > 0) ? INT_MAX : INT_MIN;
            } else {
                result->data[i][j] = a->data[i][j] + b->data[i][j];
            }
        }
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)a->data[i][k] * b->data[k][j];
                if ((product > 0 && sum > LLONG_MAX - product) ||
                    (product < 0 && sum < LLONG_MIN - product)) {
                    sum = (product > 0) ? LLONG_MAX : LLONG_MIN;
                    break;
                }
                sum += product;
            }
            if (sum > INT_MAX) result->data[i][j] = INT_MAX;
            else if (sum < INT_MIN) result->data[i][j] = INT_MIN;
            else result->data[i][j] = (int)sum;
        }
    }
}

int main(void) {
    Matrix mat1, mat2, result;
    int operation;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    init_matrix(&mat1);
    
    printf("\n");
    print_matrix(&mat1);
    
    printf("\n");
    operation = read_int("Choose operation (1=Add, 2=Multiply): ", 1, 2);
    
    if (operation == 1) {
        printf("\nMatrix Addition\n");
        printf("Matrix 2 must be %dx%d\n", mat1.rows, mat1.cols);
        mat2.rows = mat1.rows;
        mat2.cols = mat1.cols;
        init_matrix(&mat2);
        
        result.rows = mat1.rows;
        result.cols = mat1.cols;
        add_matrices(&mat1, &mat2, &result);
        
        printf("\nMatrix 1:\n");
        print_matrix(&mat1);
        printf("\nMatrix 2:\n");
        print_matrix(&mat2);
        printf("\nResult (Matrix 1 + Matrix 2):\n");
        print_matrix(&result);
        
    } else {
        printf("\nMatrix Multiplication\n");
        mat2.rows = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
        mat2.cols = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
        
        if (mat1.cols != mat2.rows) {
            printf("Error