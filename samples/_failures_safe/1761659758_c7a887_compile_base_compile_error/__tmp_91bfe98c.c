//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
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

void init_matrix(Matrix* mat) {
    int i = 0;
    while (i < mat->rows) {
        int j = 0;
        do {
            printf("Enter element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int("", INT_MIN, INT_MAX);
            j++;
        } while (j < mat->cols);
        i++;
    }
}

void print_matrix(const Matrix* mat) {
    int i, j;
    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    int i = 0;
    while (i < a->rows) {
        int j = 0;
        do {
            if (a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) {
                result->data[i][j] = INT_MAX;
            } else if (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j]) {
                result->data[i][j] = INT_MIN;
            } else {
                result->data[i][j] = a->data[i][j] + b->data[i][j];
            }
            j++;
        } while (j < a->cols);
        i++;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    int i, j, k;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < b->cols; j++) {
            long long sum = 0;
            k = 0;
            while (k < a->cols) {
                sum += (long long)a->data[i][k] * b->data[k][j];
                if (sum > INT_MAX) {
                    sum = INT_MAX;
                    break;
                }
                if (sum < INT_MIN) {
                    sum = INT_MIN;
                    break;
                }
                k++;
            }
            result->data[i][j] = (int)sum;
        }
    }
}

void transpose_matrix(const Matrix* input, Matrix* result) {
    int i = 0;
    do {
        int j = 0;
        while (j < input->cols) {
            result->data[j][i] = input->data[i][j];
            j++;
        }
        i++;
    } while (i < input->rows);
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_SIZE);
    mat1.cols = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_SIZE);
    printf("Initialize matrix 1:\n");
    init_matrix(&mat1);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    while (1) {
        printf("\nOperations:\n");
        printf("1. Add with another matrix\n");
        printf("2. Multiply with another matrix\n");
        printf("3. Transpose\n");
        printf("4. Exit\n");
        
        choice = read_int("Choose operation: ", 1, 4);
        
        if (choice == 4) {
            break;
        }
        
        switch (choice) {
            case 1: {
                mat2.rows = mat1.rows;
                mat2.cols = mat1.cols;
                printf("Initialize matrix 2 for addition:\n");
                init_matrix(&mat2);
                result.rows = mat1.rows;
                result.cols = mat1.cols;
                add_matrices(&mat1, &mat2, &result);
                printf("\nResult of addition:\n");
                print_matrix(&result);
                break;
            }
            case 2: {
                mat2.rows = read_int("Enter rows for matrix 2: ", 1, MAX_SIZE);
                mat2.cols = read_int("Enter columns for matrix 2: ", 1, MAX_SIZE);
                if (mat1.cols != mat2.rows) {
                    printf("Cannot multiply: columns of matrix 1 must equal rows of matrix 2.\n");
                    break;
                }