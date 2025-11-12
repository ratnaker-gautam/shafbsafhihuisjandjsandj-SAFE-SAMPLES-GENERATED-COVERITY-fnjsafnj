//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    long result = n * factorial(n - 1);
    if (result < 0) return -1;
    return result;
}

int fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    int a = fibonacci(n - 1);
    int b = fibonacci(n - 2);
    if (a == -1 || b == -1) return -1;
    if (a > INT_MAX - b) return -1;
    return a + b;
}

int main() {
    int num;
    char input[100];
    
    printf("Enter a positive integer (0-20 for factorial, 0-40 for Fibonacci): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &num) != 1) {
        printf("Invalid input: not a number\n");
        return 1;
    }
    
    if (num < 0) {
        printf("Error: negative numbers not allowed\n");
        return 1;
    }
    
    long fact_result = factorial(num);
    if (fact_result == -1) {
        printf("Factorial error: overflow or invalid input\n");
    } else {
        printf("Factorial of %d is %ld\n", num, fact_result);
    }
    
    int fib_result = fibonacci(num);
    if (fib_result == -1) {
        printf("Fibonacci error: overflow or invalid input\n");
    } else {
        printf("Fibonacci number at position %d is %d\n", num, fib_result);
    }
    
    return 0;
}