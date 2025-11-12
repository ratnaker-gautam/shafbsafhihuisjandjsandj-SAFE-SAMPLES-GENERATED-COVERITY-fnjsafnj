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

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    if (b == 0) return a;
    return gcd(b, a % b);
}

int read_int(const char* prompt, int min, int max) {
    printf("%s", prompt);
    int value;
    if (scanf("%d", &value) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    while (getchar() != '\n');
    if (value < min || value > max) return -1;
    return value;
}

int main() {
    printf("Numeric Computation Demo\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("3. Greatest Common Divisor\n");
    
    int choice = read_int("Choose operation (1-3): ", 1, 3);
    if (choice == -1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1: {
            int n = read_int("Enter n (0-20): ", 0, 20);
            if (n == -1) {
                printf("Invalid input\n");
                return 1;
            }
            long result = factorial(n);
            if (result == -1) {
                printf("Overflow error\n");
                return 1;
            }
            printf("Factorial(%d) = %ld\n", n, result);
            break;
        }
        case 2: {
            int n = read_int("Enter n (0-40): ", 0, 40);
            if (n == -1) {
                printf("Invalid input\n");
                return 1;
            }
            int result = fibonacci(n);
            if (result == -1) {
                printf("Overflow error\n");
                return 1;
            }
            printf("Fibonacci(%d) = %d\n", n, result);
            break;
        }
        case 3: {
            int a = read_int("Enter first number: ", INT_MIN, INT_MAX);
            int b = read_int("Enter second number: ", INT_MIN, INT_MAX);
            if (a == -1 || b == -1) {
                printf("Invalid input\n");
                return 1;
            }
            if (a == 0 && b == 0) {
                printf("GCD(0,0) is undefined\n");
                return 1;
            }
            int result = gcd(a, b);
            printf("GCD(%d, %d) = %d\n", a, b, result);
            break;
        }
    }
    
    return 0;
}