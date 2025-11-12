//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000000

int is_prime(uint64_t n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

uint64_t factorial(uint64_t n) {
    if (n > 20) return 0;
    uint64_t result = 1;
    for (uint64_t i = 1; i <= n; i++) {
        if (result > UINT64_MAX / i) return 0;
        result *= i;
    }
    return result;
}

int main() {
    int input;
    printf("Enter a positive integer (1-100): ");
    
    if (scanf("%d", &input) != 1) {
        printf("Invalid input: must be an integer\n");
        return 1;
    }
    
    if (input < 1 || input > 100) {
        printf("Invalid input: must be between 1 and 100\n");
        return 1;
    }
    
    uint64_t sum = 0;
    uint64_t count = 0;
    
    for (int i = 1; i <= input; i++) {
        uint64_t fact = factorial(i);
        if (fact == 0) {
            printf("Factorial overflow at i=%d\n", i);
            break;
        }
        
        if (is_prime(fact + 1)) {
            sum += fact;
            count++;
        }
    }
    
    printf("Sum of factorials where factorial+1 is prime: %lu\n", sum);
    printf("Count of such numbers: %lu\n", count);
    
    double ratio = 0.0;
    if (count > 0) {
        ratio = (double)sum / count;
    }
    printf("Average: %.6f\n", ratio);
    
    uint64_t fibonacci[100];
    if (input > 0) fibonacci[0] = 1;
    if (input > 1) fibonacci[1] = 1;
    
    for (int i = 2; i < input && i < 100; i++) {
        if (fibonacci[i-1] > UINT64_MAX - fibonacci[i-2]) {
            break;
        }
        fibonacci[i] = fibonacci[i-1] + fibonacci[i-2];
    }
    
    if (input > 0 && input <= 100) {
        printf("Fibonacci(%d) = %lu\n", input, fibonacci[input-1]);
    }
    
    return 0;
}