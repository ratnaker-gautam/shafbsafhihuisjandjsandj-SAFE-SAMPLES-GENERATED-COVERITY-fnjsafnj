//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_SIZE 100

double compute_polynomial(double *coeffs, int degree, double x) {
    double result = 0.0;
    double *ptr = coeffs + degree;
    
    for (int i = degree; i >= 0; i--) {
        result = result * x + *ptr;
        ptr--;
    }
    return result;
}

void fill_fibonacci(int *arr, int count) {
    if (count < 1) return;
    
    int *ptr = arr;
    *ptr = 0;
    if (count == 1) return;
    
    ptr++;
    *ptr = 1;
    if (count == 2) return;
    
    for (int i = 2; i < count; i++) {
        *(arr + i) = *(arr + i - 1) + *(arr + i - 2);
    }
}

double compute_mean(int *data, int size) {
    if (size <= 0) return 0.0;
    
    long long sum = 0;
    int *end = data + size;
    
    for (int *ptr = data; ptr < end; ptr++) {
        sum += *ptr;
    }
    
    return (double)sum / size;
}

int main() {
    int choice;
    printf("Select operation:\n1. Polynomial evaluation\n2. Fibonacci sequence\n3. Array statistics\n");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    switch (choice) {
        case 1: {
            int degree;
            printf("Enter polynomial degree (0-10): ");
            if (scanf("%d", &degree) != 1 || degree < 0 || degree > 10) {
                printf("Invalid degree\n");
                return 1;
            }
            
            double coeffs[11];
            printf("Enter %d coefficients from highest to lowest degree: ", degree + 1);
            for (int i = degree; i >= 0; i--) {
                if (scanf("%lf", coeffs + i) != 1) {
                    printf("Invalid coefficient\n");
                    return 1;
                }
            }
            
            double x;
            printf("Enter x value: ");
            if (scanf("%lf", &x) != 1) {
                printf("Invalid x value\n");
                return 1;
            }
            
            double result = compute_polynomial(coeffs, degree, x);
            printf("P(%.2f) = %.6f\n", x, result);
            break;
        }
        
        case 2: {
            int count;
            printf("Enter number of Fibonacci numbers to generate (1-50): ");
            if (scanf("%d", &count) != 1 || count < 1 || count > 50) {
                printf("Invalid count\n");
                return 1;
            }
            
            int fib[50];
            fill_fibonacci(fib, count);
            
            printf("Fibonacci sequence: ");
            for (int i = 0; i < count; i++) {
                printf("%d ", *(fib + i));
            }
            printf("\n");
            break;
        }
        
        case 3: {
            int size;
            printf("Enter array size (1-100): ");
            if (scanf("%d", &size) != 1 || size < 1 || size > 100) {
                printf("Invalid size\n");
                return 1;
            }
            
            int data[ARRAY_SIZE];
            printf("Enter %d integers: ", size);
            for (int i = 0; i < size; i++) {
                if (scanf("%d", data + i) != 1) {
                    printf("Invalid input\n");
                    return 1;
                }
            }
            
            double mean = compute_mean(data, size);
            printf("Mean: %.2f\n", mean);
            
            int min = *data;
            int max = *data;
            int *end = data + size;
            
            for (int *ptr = data + 1; ptr < end; ptr++) {
                if (*ptr < min) min = *ptr;
                if (*ptr > max) max = *ptr;
            }
            
            printf("Min: %d, Max: %d\n", min, max);
            break;
        }
        
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}