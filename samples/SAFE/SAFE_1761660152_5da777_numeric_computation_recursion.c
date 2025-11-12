//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    long long result = n * factorial(n - 1);
    if (result < 0) return -1;
    return result;
}

long long fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    long long a = fibonacci(n - 1);
    long long b = fibonacci(n - 2);
    if (a == -1 || b == -1) return -1;
    long long result = a + b;
    if (result < a) return -1;
    return result;
}

int read_positive_int(const char* prompt, int max_val) {
    printf("%s", prompt);
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    char* endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (val < 0 || val > max_val) {
        return -1;
    }
    return (int)val;
}

int main(void) {
    printf("Recursive Numeric Computation\n");
    printf("==============================\n");
    
    int choice = read_positive_int("Choose operation (1=Factorial, 2=Fibonacci): ", 2);
    if (choice == -1) {
        printf("Invalid choice input.\n");
        return 1;
    }
    
    int max_n = (choice == 1) ? 20 : 40;
    int n = read_positive_int("Enter n: ", max_n);
    if (n == -1) {
        printf("Invalid n input.\n");
        return 1;
    }
    
    long long result;
    if (choice == 1) {
        result = factorial(n);
        if (result == -1) {
            printf("Factorial computation failed (overflow).\n");
            return 1;
        }
        printf("Factorial(%d) = %lld\n", n, result);
    } else {
        result = fibonacci(n);
        if (result == -1) {
            printf("Fibonacci computation failed (overflow).\n");
            return 1;
        }
        printf("Fibonacci(%d) = %lld\n", n, result);
    }
    
    return 0;
}