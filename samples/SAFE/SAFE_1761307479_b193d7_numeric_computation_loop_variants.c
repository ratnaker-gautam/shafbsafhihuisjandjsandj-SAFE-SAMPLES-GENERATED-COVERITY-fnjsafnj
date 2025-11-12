//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 100

int main() {
    int n;
    printf("Enter number of elements (1-%d): ", MAX_SIZE);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_SIZE) {
        fprintf(stderr, "Error: Size out of bounds\n");
        return 1;
    }
    
    double arr[MAX_SIZE];
    printf("Enter %d numbers:\n", n);
    
    int i = 0;
    while (i < n) {
        if (scanf("%lf", &arr[i]) != 1) {
            fprintf(stderr, "Error: Invalid number\n");
            return 1;
        }
        i++;
    }
    
    double sum = 0.0;
    double min_val = arr[0];
    double max_val = arr[0];
    
    for (i = 0; i < n; i++) {
        sum += arr[i];
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
    }
    
    double mean = sum / n;
    
    double variance = 0.0;
    int j = 0;
    do {
        double diff = arr[j] - mean;
        variance += diff * diff;
        j++;
    } while (j < n);
    variance /= n;
    
    printf("Statistical summary:\n");
    printf("Count: %d\n", n);
    printf("Minimum: %.6f\n", min_val);
    printf("Maximum: %.6f\n", max_val);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    
    printf("Factorials (modulo 1000000):\n");
    for (i = 1; i <= n && i <= 20; i++) {
        unsigned long long fact = 1;
        int k = 1;
        while (k <= i) {
            if (fact > ULLONG_MAX / k) {
                fact %= 1000000;
            }
            fact *= k;
            k++;
        }
        fact %= 1000000;
        printf("%d! mod 1000000 = %llu\n", i, fact);
    }
    
    printf("Prime check for first %d numbers:\n", n);
    int count = 0;
    for (i = 2; count < n && i < 1000; i++) {
        int is_prime = 1;
        int divisor = 2;
        while (divisor * divisor <= i) {
            if (i % divisor == 0) {
                is_prime = 0;
                break;
            }
            divisor++;
        }
        if (is_prime) {
            printf("%d is prime\n", i);
            count++;
        }
    }
    
    return 0;
}