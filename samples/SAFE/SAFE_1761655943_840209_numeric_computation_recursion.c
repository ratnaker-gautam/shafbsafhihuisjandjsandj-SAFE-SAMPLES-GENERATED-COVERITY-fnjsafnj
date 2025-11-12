//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    
    long long result = factorial(n - 1);
    if (result > LLONG_MAX / n) return -1;
    return result * n;
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

int read_positive_int(const char* prompt) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        if (value < 0) {
            printf("Please enter a non-negative integer.\n");
            continue;
        }
        
        return value;
    }
}

int main() {
    int choice;
    
    while (1) {
        printf("\nRecursive Numeric Computation Menu:\n");
        printf("1. Factorial\n");
        printf("2. Fibonacci\n");
        printf("3. Greatest Common Divisor\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");
        
        char buffer[100];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 4.\n");
            continue;
        }
        
        if (choice == 4) {
            break;
        }
        
        switch (choice) {
            case 1: {
                int n = read_positive_int("Enter a number for factorial (0-20): ");
                if (n == -1) continue;
                if (n > 20) {
                    printf("Number too large for factorial calculation.\n");
                    continue;
                }
                long long result = factorial(n);
                if (result == -1) {
                    printf("Overflow occurred in factorial calculation.\n");
                } else {
                    printf("%d! = %lld\n", n, result);
                }
                break;
            }
            case 2: {
                int n = read_positive_int("Enter Fibonacci sequence position (0-40): ");
                if (n == -1) continue;
                if (n > 40) {
                    printf("Number too large for Fibonacci calculation.\n");
                    continue;
                }
                int result = fibonacci(n);
                if (result == -1) {
                    printf("Overflow occurred in Fibonacci calculation.\n");
                } else {
                    printf("Fibonacci(%d) = %d\n", n, result);
                }
                break;
            }
            case 3: {
                int a = read_positive_int("Enter first number for GCD: ");
                if (a == -1) continue;
                int b = read_positive_int("Enter second number for GCD: ");
                if (b == -1) continue;
                int result = gcd(a, b);
                printf("GCD(%d, %d) = %d\n", a, b, result);
                break;
            }
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}