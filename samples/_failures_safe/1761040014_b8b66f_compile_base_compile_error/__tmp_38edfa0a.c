//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM * MAX_DIM];
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
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        printf("Element [%d][%d]: ", i / mat->cols, i % mat->cols);
        *(mat->data + i) = read_int("", INT_MIN, INT_MAX);
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", *(mat->data + i * mat->cols + j));
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += *(a->data + i * a->cols + k) * *(b->data + k * b->cols + j);
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
    
    return 1;
}

int add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows * a->cols; i++) {
        *(result->data + i) = *(a->data + i) + *(b->data + i);
    }
    
    return 1;
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest->data + j * dest->cols + i) = *(src->data + i * src->cols + j);
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    printf("Matrix 1 dimensions:\n");
    mat1.rows = read_int("Rows (1-10): ", 1, MAX_DIM);
    mat1.cols = read_int("Columns (1-10): ", 1, MAX_DIM);
    init_matrix(&mat1);
    
    printf("Matrix 2 dimensions:\n");
    mat2.rows = read_int("Rows (1-10): ", 1, MAX_DIM);
    mat2.cols = read_int("Columns (1-10): ", 1, MAX_DIM);
    init_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    while (1) {
        printf("\nOperations:\n");
        printf("1. Matrix Addition\n");
        printf("2. Matrix Multiplication\n");
        printf("3. Transpose Matrix 1\n");
        printf("4. Transpose Matrix 2\n");
        printf("5. Exit\n");
        
        choice = read_int("Select operation: ", 1, 5);
        
        if (choice == 5) {
            break;
        }
        
        switch (choice) {
            case 1:
                if (add_matrices(&mat1, &mat2, &result)) {
                    printf("\nMatrix Addition Result:\n");
                    print_matrix(&result);
                } else {
                    printf("Error: Matrices must have same dimensions for addition.\n");
                }
                break;
                
            case 2:
                if (multiply_matrices(&mat1, &mat2, &result)) {
                    printf("\nMatrix Multiplication Result:\n");
                    print_matrix(&result);
                } else {
                    printf("Error: Matrix 1 columns must equal Matrix 2 rows for multiplication.\n");
                }
                break;
                
            case 3:
                transpose_matrix