//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>

#define MAX_ITERATIONS 1000000

int is_prime(uint64_t n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    
    uint64_t limit = (uint64_t)sqrt((double)n) + 1;
    for (uint64_t i = 3; i <= limit; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

uint64_t find_nth_prime(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 2;
    
    int count = 1;
    uint64_t candidate = 3;
    
    while (count < n) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        
        if (candidate > UINT64_MAX - 2) {
            fprintf(stderr, "Integer overflow detected\n");
            exit(1);
        }
        candidate += 2;
        
        if (count > MAX_ITERATIONS) {
            fprintf(stderr, "Maximum iterations exceeded\n");
            exit(1);
        }
    }
    return candidate;
}

int main() {
    int n;
    char input[32];
    
    printf("Enter which prime number to find (1-10000): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > 10000) {
        fprintf(stderr, "Input out of range (1-10000)\n");
        return 1;
    }
    
    uint64_t result = find_nth_prime(n);
    
    if (result == 0) {
        fprintf(stderr, "Error computing prime\n");
        return 1;
    }
    
    printf("The %d%s prime number is: %lu\n", 
           n, 
           (n % 10 == 1 && n % 100 != 11) ? "st" : 
           (n % 10 == 2 && n % 100 != 12) ? "nd" : 
           (n % 10 == 3 && n % 100 != 13) ? "rd" : "th", 
           result);
    
    return 0;
}