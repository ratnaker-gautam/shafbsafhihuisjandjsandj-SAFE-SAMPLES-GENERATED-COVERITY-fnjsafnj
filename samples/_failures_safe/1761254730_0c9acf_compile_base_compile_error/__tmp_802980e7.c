//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
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
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

void input_matrix(Matrix* mat) {
    printf("Enter matrix elements (%dx%d):\n", mat->rows, mat->cols);
    
    int i = 0;
    while (i < mat->rows) {
        int j = 0;
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
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    int i = 0;
    do {
        int j = 0;
        do {
            if (a->rows == b->rows && a->cols == b->cols) {
                result->data[i][j] = a->data[i][j] + b->data[i][j];
            }
            j++;
        } while (j < a->cols);
        i++;
    } while (i < a->rows);
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return;
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            int k = 0;
            while (k < a->cols) {
                sum += a->data[i][k] * b->data[k][j];
                k++;
            }
            result->data[i][j] = sum;
        }
    }
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    int i = 0;
    while (i < src->rows) {
        int j = 0;
        while (j < src->cols) {
            dest->data[j][i] = src->data[i][j];
            j++;
        }
        i++;
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    init_matrix(&mat1);
    input_matrix(&mat1);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    mat2.rows = read_int("\nEnter rows for matrix 2 (1-10): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
    init_matrix(&mat2);
    input_matrix(&mat2);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    while (1) {
        printf("\nMatrix Operations:\n");
        printf("1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Transpose matrix 1\n");
        printf("4. Transpose matrix 2\n");
        printf("5. Exit\n");
        
        choice = read_int("Select operation: ", 1, 5);
        
        if (choice == 5) {
            break;
        }
        
        switch (choice) {
            case 1:
                if (mat1.rows == mat2.rows && mat1.cols == mat2.cols) {
                    result.rows = mat1.rows;
                    result.cols = mat1.cols;
                    init_matrix(&result);
                    add_matrices(&mat1, &mat2, &result);
                    printf("\nMatrix addition result:\n");
                    print_matrix(&result);
                } else {
                    printf("Matrices must have same dimensions for addition.\n");
                }
                break;