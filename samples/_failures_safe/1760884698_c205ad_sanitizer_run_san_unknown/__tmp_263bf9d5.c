//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    int n;
    printf("Enter number of elements (1-100): ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 100) {
        fprintf(stderr, "Error: Number out of range\n");
        return 1;
    }
    
    long long sum = 0;
    long long product = 1;
    int max_val = INT_MIN;
    int min_val = INT_MAX;
    
    printf("Enter %d integers:\n", n);
    
    int i = 0;
    while (i < n) {
        int num;
        if (scanf("%d", &num) != 1) {
            fprintf(stderr, "Error: Invalid input\n");
            return 1;
        }
        
        if (num > max_val) max_val = num;
        if (num < min_val) min_val = num;
        
        sum += num;
        
        if (num != 0) {
            if (product > LLONG_MAX / abs(num)) {
                fprintf(stderr, "Error: Product overflow\n");
                return 1;
            }
            product *= num;
        }
        
        i++;
    }
    
    double average = (double)sum / n;
    
    printf("\nResults:\n");
    printf("Sum: %lld\n", sum);
    printf("Product: %lld\n", product);
    printf("Average: %.2f\n", average);
    printf("Maximum: %d\n", max_val);
    printf("Minimum: %d\n", min_val);
    
    long long factorial = 1;
    int j;
    for (j = 1; j <= abs(min_val) && j <= 20; j++) {
        if (factorial > LLONG_MAX / j) {
            factorial = -1;
            break;
        }
        factorial *= j;
    }
    
    if (min_val >= 0 && min_val <= 20) {
        printf("Factorial of minimum (%d): %lld\n", min_val, factorial);
    } else {
        printf("Factorial calculation skipped (value out of range)\n");
    }
    
    int k = 0;
    long long fibonacci_prev = 0;
    long long fibonacci_curr = 1;
    long long fibonacci_next;
    
    printf("Fibonacci sequence up to %d terms:\n", n > 10 ? 10 : n);
    do {
        printf("%lld ", fibonacci_prev);
        if (k == 0) {
            fibonacci_next = 1;
        } else {
            fibonacci_next = fibonacci_prev + fibonacci_curr;
            fibonacci_prev = fibonacci_curr;
            fibonacci_curr = fibonacci_next;
        }
        k++;
    } while (k < (n > 10 ? 10 : n));
    printf("\n");
    
    return 0;
}