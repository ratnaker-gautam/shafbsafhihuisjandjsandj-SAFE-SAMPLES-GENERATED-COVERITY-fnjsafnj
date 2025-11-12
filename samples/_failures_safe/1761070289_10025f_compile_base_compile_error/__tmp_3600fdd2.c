//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

int main() {
    int n;
    printf("Enter a positive integer (1-%d): ", MAX_INPUT);
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of range\n");
        return 1;
    }
    
    long long sum = 0;
    long long square_sum = 0;
    double harmonic_sum = 0.0;
    
    int i = 1;
    while (i <= n) {
        sum += i;
        i++;
    }
    
    for (int j = 1; j <= n; j++) {
        square_sum += (long long)j * j;
    }
    
    int k = 1;
    do {
        harmonic_sum += 1.0 / k;
        k++;
    } while (k <= n);
    
    double arithmetic_mean = (double)sum / n;
    double quadratic_mean = sqrt((double)square_sum / n);
    double harmonic_mean = n / harmonic_sum;
    
    printf("Results for n = %d:\n", n);
    printf("Sum (1 to n): %lld\n", sum);
    printf("Sum of squares: %lld\n", square_sum);
    printf("Arithmetic mean: %.6f\n", arithmetic_mean);
    printf("Quadratic mean: %.6f\n", quadratic_mean);
    printf("Harmonic mean: %.6f\n", harmonic_mean);
    
    printf("\nPrime numbers up to %d:\n", n);
    if (n >= 2) {
        printf("2");
        for (int num = 3; num <= n; num += 2) {
            int is_prime = 1;
            int limit = (int)sqrt(num) + 1;
            for (int divisor = 3; divisor <= limit; divisor += 2) {
                if (num % divisor == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                printf(", %d", num);
            }
        }
        printf("\n");
    }
    
    printf("\nCollatz sequence for %d:\n", n);
    int current = n;
    int steps = 0;
    while (current != 1 && steps < MAX_ITERATIONS) {
        printf("%d ", current);
        if (current % 2 == 0) {
            current /= 2;
        } else {
            if (current > (INT_MAX - 1) / 3) {
                fprintf(stderr, "Error: Integer overflow in Collatz sequence\n");
                return 1;
            }
            current = current * 3 + 1;
        }
        steps++;
    }
    if (steps < MAX_ITERATIONS) {
        printf("1\n");
        printf("Steps: %d\n", steps);
    } else {
        printf("... (max iterations reached)\n");
    }
    
    return 0;
}