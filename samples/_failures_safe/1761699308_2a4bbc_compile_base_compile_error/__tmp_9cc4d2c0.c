//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
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

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_DIM || cols > MAX_DIM) return 0;
    return 1;
}

int read_integer(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer) continue;
        if (*endptr != '\n' && *endptr != '\0') continue;
        if (value < min_val || value > max_val) continue;
        if (value > INT_MAX || value < INT_MIN) continue;
        
        return (int)value;
    }
}

void read_matrix_element(Matrix* mat, int i, int j) {
    char prompt[64];
    snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
    mat->data[i][j] = read_integer(prompt, -1000, 1000);
}

void input_matrix(Matrix* mat) {
    printf("Enter matrix dimensions:\n");
    mat->rows = read_integer("Rows: ", 1, MAX_DIM);
    mat->cols = read_integer("Cols: ", 1, MAX_DIM);
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            read_matrix_element(mat, i, j);
        }
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

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) return 0;
    if (!validate_matrix_dimensions(a->rows, b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) return 0;
                if (sum > INT_MAX - product || sum < INT_MIN - product) return 0;
                sum += product;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix* src, Matrix* dest) {
    if (!validate_matrix_dimensions(src->cols, src->rows)) return 0;
    
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
    return 1;
}

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long sum = (long)a->data[i][j] + (long)b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) return 0;
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

void display_menu() {
    printf("\nMatrix Operations Menu:\n");
    printf("1. Input matrices\n");
    printf("2. Matrix multiplication\n");
    printf("3. Matrix transpose\n");
    printf("4. Matrix addition\n");
    printf("5. Display matrices\n");
    printf("6. Exit\n");
    printf("Choose operation: ");
}

int main() {
    Matrix mat1 = {0}, mat2 = {0}, result = {0};
    int choice;
    int matrices_initialized = 0;
    
    while (1) {
        display_menu();
        choice = read_integer("", 1, 6);
        
        switch (choice) {
            case 1:
                printf("Matrix 1:\n");
                input_matrix(&mat1);
                printf("Matrix 2:\n");
                input