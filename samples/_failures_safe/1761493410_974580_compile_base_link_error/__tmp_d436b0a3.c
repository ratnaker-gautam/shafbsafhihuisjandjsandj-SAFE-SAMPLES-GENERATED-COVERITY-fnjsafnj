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
    uint64_t candidate = 1;
    
    while (count < n) {
        candidate += 2;
        if (is_prime(candidate)) {
            count++;
        }
        if (candidate >= UINT64_MAX - 2) {
            return 0;
        }
    }
    return candidate;
}

int main() {
    int input;
    char buffer[32];
    
    printf("Enter which prime number to find (1-10000): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(buffer, "%d", &input) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (input < 1 || input > 10000) {
        fprintf(stderr, "Input must be between 1 and 10000\n");
        return 1;
    }
    
    uint64_t result = find_nth_prime(input);
    
    if (result == 0) {
        fprintf(stderr, "Computation failed - number too large\n");
        return 1;
    }
    
    printf("The %d%s prime number is: %lu\n", 
           input, 
           (input % 10 == 1 && input % 100 != 11) ? "st" : 
           (input % 10 == 2 && input % 100 != 12) ? "nd" : 
           (input % 10 == 3 && input % 100 != 13) ? "rd" : "th", 
           result);
    
    return 0;
}