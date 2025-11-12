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

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
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

void init_matrix(Matrix* mat) {
    printf("Enter values for %dx%d matrix:\n", mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            int* elem = mat->data + i * mat->cols + j;
            *elem = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            const int* elem = mat->data + i * mat->cols + j;
            printf("%6d ", *elem);
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
            int* res_elem = result->data + i * result->cols + j;
            *res_elem = 0;
            
            for (int k = 0; k < a->cols; k++) {
                const int* a_elem = a->data + i * a->cols + k;
                const int* b_elem = b->data + k * b->cols + j;
                
                if (*a_elem > 0 && *b_elem > 0) {
                    if (*a_elem > INT_MAX / *b_elem) {
                        return 0;
                    }
                } else if (*a_elem < 0 && *b_elem < 0) {
                    if (*a_elem < INT_MAX / *b_elem) {
                        return 0;
                    }
                } else if (*a_elem != 0 && *b_elem != 0) {
                    if ((*a_elem == -1 && *b_elem == INT_MIN) ||
                        (*b_elem == -1 && *a_elem == INT_MIN)) {
                        return 0;
                    }
                }
                
                int product = *a_elem * *b_elem;
                if ((product > 0 && *res_elem > INT_MAX - product) ||
                    (product < 0 && *res_elem < INT_MIN - product)) {
                    return 0;
                }
                
                *res_elem += product;
            }
        }
    }
    
    return 1;
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            const int* src_elem = src->data + i * src->cols + j;
            int* dest_elem = dest->data + j * dest->cols + i;
            *dest_elem = *src_elem;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    init_matrix(&mat1);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\n");
    mat2.rows = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    init_matrix(&mat2);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix Operations