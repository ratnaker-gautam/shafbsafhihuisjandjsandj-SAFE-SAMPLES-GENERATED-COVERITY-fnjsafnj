//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    if (n > 20) return -1;
    long long result = n * factorial(n - 1);
    if (result < 0) return -1;
    return result;
}

int fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n > 46) return -1;
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
    if (a == 0) return b;
    return gcd(b, a % b);
}

int main() {
    int choice, num1, num2, result_int;
    long long result_ll;
    
    printf("Numeric Computation Menu:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("3. GCD\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Enter number (0-20): ");
            if (scanf("%d", &num1) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            result_ll = factorial(num1);
            if (result_ll == -1) {
                printf("Invalid input or overflow\n");
            } else {
                printf("Factorial of %d is %lld\n", num1, result_ll);
            }
            break;
            
        case 2:
            printf("Enter Fibonacci position (0-46): ");
            if (scanf("%d", &num1) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            result_int = fibonacci(num1);
            if (result_int == -1) {
                printf("Invalid input or overflow\n");
            } else {
                printf("Fibonacci at position %d is %d\n", num1, result_int);
            }
            break;
            
        case 3:
            printf("Enter two numbers: ");
            if (scanf("%d %d", &num1, &num2) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result_int = gcd(num1, num2);
            printf("GCD of %d and %d is %d\n", num1, num2, result_int);
            break;
            
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}