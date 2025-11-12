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
        printf("Invalid input. Enter a number between %d and %d.\n", min_val, max_val);
    }
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

void print_matrix(const Matrix* mat) {
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
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
                result->data[i][j] = 0;
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
                long long product = (long long)a->data[i][k] * (long long)b->data[k][j];
                if (product > INT_MAX - sum || product < INT_MIN + sum) {
                    sum = 0;
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

void transpose_matrix(const Matrix* input, Matrix* result) {
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    printf("Enter dimensions for first matrix:\n");
    mat1.rows = read_int("Rows (1-10): ", 1, MAX_DIM);
    mat1.cols = read_int("Columns (1-10): ", 1, MAX_DIM);
    read_matrix(&mat1);
    
    printf("Enter dimensions for second matrix:\n");
    mat2.rows = read_int("Rows (1-10): ", 1, MAX_DIM);
    mat2.cols = read_int("Columns (1-10): ", 1, MAX_DIM);
    read_matrix(&mat2);
    
    while (1) {
        printf("\nMatrix Operations Menu:\n");
        printf("1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Transpose first matrix\n");
        printf("4. Transpose second matrix\n");
        printf("5. Display matrices\n");
        printf("6. Exit\n");
        
        choice = read_int("Choose operation: ", 1, 6);
        
        switch (choice) {
            case 1:
                if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
                    printf("Error: Matrices must have same dimensions for addition.\n");
                    break;
                }
                result.rows = mat1.rows;
                result.cols = mat1.cols;
                add_matrices(&mat1, &mat2, &result);
                printf("Matrix addition result:\n");
                print_matrix(&result);
                break;
                
            case 2:
                if (mat1.cols !=