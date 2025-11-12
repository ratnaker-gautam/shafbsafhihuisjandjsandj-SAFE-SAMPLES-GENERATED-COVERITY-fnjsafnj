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

int read_int(const char* prompt) {
    int value;
    char buffer[100];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    if (sscanf(buffer, "%d", &value) != 1) return -1;
    return value;
}

int main() {
    int choice;
    printf("Numeric Computation Menu:\n");
    printf("1. Factorial\n");
    printf("2. Fibonacci\n");
    printf("3. GCD\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    switch (choice) {
        case 1: {
            int n = read_int("Enter non-negative integer: ");
            if (n < 0 || n > 20) {
                printf("Input out of range (0-20).\n");
                break;
            }
            long result = factorial(n);
            if (result == -1) {
                printf("Overflow occurred.\n");
            } else {
                printf("Factorial(%d) = %ld\n", n, result);
            }
            break;
        }
        case 2: {
            int n = read_int("Enter non-negative integer: ");
            if (n < 0 || n > 40) {
                printf("Input out of range (0-40).\n");
                break;
            }
            int result = fibonacci(n);
            if (result == -1) {
                printf("Overflow occurred.\n");
            } else {
                printf("Fibonacci(%d) = %d\n", n, result);
            }
            break;
        }
        case 3: {
            int a = read_int("Enter first integer: ");
            int b = read_int("Enter second integer: ");
            if (a == INT_MIN || b == INT_MIN) {
                printf("Input out of range.\n");
                break;
            }
            int result = gcd(a, b);
            printf("GCD(%d, %d) = %d\n", a, b, result);
            break;
        }
        default:
            printf("Invalid choice.\n");
            break;
    }
    
    return 0;
}