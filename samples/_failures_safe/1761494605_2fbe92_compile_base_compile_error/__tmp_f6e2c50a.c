//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM * MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements row by row:\n");
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *(mat->data + i * mat->cols + j) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(mat->data + i * mat->cols + j));
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
                int term1 = *(a->data + i * a->cols + k);
                int term2 = *(b->data + k * b->cols + j);
                
                if ((term1 > 0 && term2 > 0 && term1 > INT_MAX / term2) ||
                    (term1 > 0 && term2 < 0 && term2 < INT_MIN / term1) ||
                    (term1 < 0 && term2 > 0 && term1 < INT_MIN / term2) ||
                    (term1 < 0 && term2 < 0 && term1 < INT_MAX / term2)) {
                    return 0;
                }
                
                int product = term1 * term2;
                
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                
                sum += product;
            }
            *(result->data + i * result->cols + j) = sum;
        }
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
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    
    choice = read_int("Enter choice (1-2): ", 1, 2);
    if (choice == -1) return 1;
    
    if (choice == 1) {
        printf("Matrix 1:\n");
        mat1.rows = read_int("Enter rows: ", 1, MAX_DIM);
        if (mat1.rows == -1) return 1;
        mat1.cols = read_int("Enter columns: ", 1, MAX_DIM);
        if (mat1.cols == -1) return 1;
        read_matrix(&mat1);
        
        printf("Matrix 2:\n");
        mat2.rows = read_int("Enter rows: ", 1, MAX_DIM);
        if (mat2.rows == -1) return 1;
        mat2.cols = read_int("Enter columns: ", 1, MAX_DIM);
        if (mat2.cols == -1) return 1;
        read_matrix(&mat2);
        
        printf("\nMatrix 1:\n");
        print_matrix(&mat1);
        printf("\nMatrix 2:\n");
        print_matrix(&mat2);
        
        if (multiply_mat