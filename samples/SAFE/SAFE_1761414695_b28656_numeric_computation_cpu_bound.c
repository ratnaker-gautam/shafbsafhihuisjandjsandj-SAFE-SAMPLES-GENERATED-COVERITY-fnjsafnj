//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000000

int main() {
    int n;
    printf("Enter a positive integer (1-1000): ");
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > 1000) {
        fprintf(stderr, "Error: Number must be between 1 and 1000\n");
        return 1;
    }
    
    long double pi_approx = 0.0L;
    int sign = 1;
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (i > 0 && i % 100000 == 0) {
            printf("Progress: %d/%d iterations\n", i, MAX_ITERATIONS);
        }
        
        long double denominator = 2.0L * i + 1.0L;
        if (denominator == 0.0L) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        
        long double term = 1.0L / denominator;
        if (sign > 0) {
            if (pi_approx > LDBL_MAX - term) {
                fprintf(stderr, "Error: Pi approximation would overflow\n");
                return 1;
            }
            pi_approx += term;
        } else {
            if (pi_approx < -LDBL_MAX + term) {
                fprintf(stderr, "Error: Pi approximation would underflow\n");
                return 1;
            }
            pi_approx -= term;
        }
        sign = -sign;
    }
    
    if (pi_approx > LDBL_MAX / 4.0L) {
        fprintf(stderr, "Error: Pi multiplication would overflow\n");
        return 1;
    }
    pi_approx *= 4.0L;
    
    printf("Approximation of π using %d terms: %.15Lf\n", MAX_ITERATIONS, pi_approx);
    
    long double sum = 0.0L;
    for (int i = 1; i <= n; i++) {
        long double divisor = (long double)i * i;
        if (divisor == 0.0L) {
            fprintf(stderr, "Error: Division by zero\n");
            return 1;
        }
        long double term = 1.0L / divisor;
        if (sum > LDBL_MAX - term) {
            fprintf(stderr, "Error: Sum would overflow\n");
            return 1;
        }
        sum += term;
    }
    
    long double basel_result = 0.0L;
    for (int i = 0; i < 6; i++) {
        if (basel_result > LDBL_MAX - sum) {
            fprintf(stderr, "Error: Basel result would overflow\n");
            return 1;
        }
        basel_result += sum;
    }
    
    if (basel_result > LDBL_MAX / 6.0L) {
        fprintf(stderr, "Error: Basel division would overflow\n");
        return 1;
    }
    basel_result /= 6.0L;
    
    printf("Basel problem approximation for n=%d: %.15Lf\n", n, basel_result);
    
    if ((pi_approx > 0 && basel_result > 0 && pi_approx > LDBL_MAX / basel_result) ||
        (pi_approx < 0 && basel_result < 0 && pi_approx < LDBL_MAX / basel_result) ||
        (pi_approx > 0 && basel_result < 0 && pi_approx < -LDBL_MAX / basel_result) ||
        (pi_approx < 0 && basel_result > 0 && pi_approx < -LDBL_MAX / basel_result)) {
        fprintf(stderr, "Error: Final multiplication would overflow\n");
        return 1;
    }
    long double final_result = pi_approx * basel_result;
    printf("Final computed value: %.15Lf\n", final_result);
    
    return 0;
}