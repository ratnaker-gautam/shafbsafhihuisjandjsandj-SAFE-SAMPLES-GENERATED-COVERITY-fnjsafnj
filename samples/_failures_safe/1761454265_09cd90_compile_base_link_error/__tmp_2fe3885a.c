//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

int main() {
    double principal, rate, final_amount;
    int years;
    char input[256];
    
    printf("Enter principal amount: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input, "%lf", &principal) != 1) {
        fprintf(stderr, "Invalid principal amount\n");
        return 1;
    }
    if (principal <= 0) {
        fprintf(stderr, "Principal must be positive\n");
        return 1;
    }
    
    printf("Enter annual interest rate (%%): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input, "%lf", &rate) != 1) {
        fprintf(stderr, "Invalid interest rate\n");
        return 1;
    }
    if (rate <= 0 || rate > 1000) {
        fprintf(stderr, "Interest rate must be between 0 and 1000\n");
        return 1;
    }
    
    printf("Enter number of years: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input, "%d", &years) != 1) {
        fprintf(stderr, "Invalid number of years\n");
        return 1;
    }
    if (years <= 0 || years > 1000) {
        fprintf(stderr, "Years must be between 1 and 1000\n");
        return 1;
    }
    
    rate = rate / 100.0;
    
    if (principal > 1e308 / pow(1.0 + rate, years)) {
        fprintf(stderr, "Calculation would overflow\n");
        return 1;
    }
    
    final_amount = principal * pow(1.0 + rate, years);
    
    if (final_amount > 1e308) {
        fprintf(stderr, "Result too large\n");
        return 1;
    }
    
    printf("Principal: $%.2lf\n", principal);
    printf("Annual Rate: %.2lf%%\n", rate * 100);
    printf("Years: %d\n", years);
    printf("Final Amount: $%.2lf\n", final_amount);
    
    return 0;
}