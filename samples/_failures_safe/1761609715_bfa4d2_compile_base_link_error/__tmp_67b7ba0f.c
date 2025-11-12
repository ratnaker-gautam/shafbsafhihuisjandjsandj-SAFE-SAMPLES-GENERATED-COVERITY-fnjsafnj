//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INPUT 100

int main() {
    char input[MAX_INPUT + 1];
    int n;
    
    printf("Enter a positive integer (1-100): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > 100) {
        fprintf(stderr, "Input out of range\n");
        return 1;
    }
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    long long sum1 = 0;
    int i = 1;
    printf("While loop: ");
    while (i <= n) {
        if (i > 1) printf(" + ");
        printf("%d²", i);
        sum1 += (long long)i * i;
        i++;
    }
    printf(" = %lld\n", sum1);
    
    long long sum2 = 0;
    printf("For loop: ");
    for (int j = 1; j <= n; j++) {
        if (j > 1) printf(" + ");
        printf("%d²", j);
        sum2 += (long long)j * j;
    }
    printf(" = %lld\n", sum2);
    
    long long sum3 = 0;
    int k = 1;
    printf("Do-while loop: ");
    do {
        if (k > 1) printf(" + ");
        printf("%d²", k);
        sum3 += (long long)k * k;
        k++;
    } while (k <= n);
    printf(" = %lld\n", sum3);
    
    long long expected = (long long)n * (n + 1) * (2 * n + 1) / 6;
    printf("\nVerification using formula n(n+1)(2n+1)/6: %lld\n", expected);
    
    if (sum1 == expected && sum2 == expected && sum3 == expected) {
        printf("All loop variants produced correct result!\n");
    } else {
        printf("Error: Loop results don't match formula!\n");
        return 1;
    }
    
    printf("\nComputing approximations of π using different series:\n");
    
    double pi_approx1 = 0.0;
    int terms1 = 1000;
    for (int m = 0; m < terms1; m++) {
        pi_approx1 += pow(-1, m) / (2.0 * m + 1);
    }
    pi_approx1 *= 4.0;
    printf("Leibniz series (%d terms): %.10f\n", terms1, pi_approx1);
    
    double pi_approx2 = 0.0;
    int terms2 = 100;
    for (int m = 1; m <= terms2; m++) {
        pi_approx2 += 1.0 / ((double)m * m);
    }
    pi_approx2 = sqrt(pi_approx2 * 6.0);
    printf("Basel problem (%d terms): %.10f\n", terms2, pi_approx2);
    
    double pi_actual = 3.14159265358979323846;
    printf("Actual π: %.10f\n", pi_actual);
    
    double error1 = fabs(pi_approx1 - pi_actual);
    double error2 = fabs(pi_approx2 - pi_actual);
    printf("Absolute errors: Leibniz=%.10f, Basel=%.10f\n", error1, error2);
    
    return 0;
}