//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    double principal, rate, result;
    int years, compounds;
    
    printf("Enter principal amount: ");
    if (scanf("%lf", &principal) != 1) {
        fprintf(stderr, "Error: Invalid principal amount\n");
        return 1;
    }
    
    if (principal <= 0) {
        fprintf(stderr, "Error: Principal must be positive\n");
        return 1;
    }
    
    printf("Enter annual interest rate (%%): ");
    if (scanf("%lf", &rate) != 1) {
        fprintf(stderr, "Error: Invalid interest rate\n");
        return 1;
    }
    
    if (rate < 0 || rate > 100) {
        fprintf(stderr, "Error: Interest rate must be between 0 and 100\n");
        return 1;
    }
    
    printf("Enter number of years: ");
    if (scanf("%d", &years) != 1) {
        fprintf(stderr, "Error: Invalid number of years\n");
        return 1;
    }
    
    if (years <= 0 || years > 100) {
        fprintf(stderr, "Error: Years must be between 1 and 100\n");
        return 1;
    }
    
    printf("Enter compounding frequency per year: ");
    if (scanf("%d", &compounds) != 1) {
        fprintf(stderr, "Error: Invalid compounding frequency\n");
        return 1;
    }
    
    if (compounds <= 0 || compounds > 365) {
        fprintf(stderr, "Error: Compounding frequency must be between 1 and 365\n");
        return 1;
    }
    
    double decimal_rate = rate / 100.0;
    double base = 1.0 + (decimal_rate / compounds);
    double exponent = compounds * years;
    
    if (exponent > 10000) {
        fprintf(stderr, "Error: Calculation would exceed safe limits\n");
        return 1;
    }
    
    result = principal * pow(base, exponent);
    
    if (result == HUGE_VAL || result == -HUGE_VAL || result != result) {
        fprintf(stderr, "Error: Calculation resulted in overflow or invalid value\n");
        return 1;
    }
    
    printf("\nCompound Interest Calculation Results:\n");
    printf("Principal: $%.2lf\n", principal);
    printf("Annual Rate: %.2lf%%\n", rate);
    printf("Years: %d\n", years);
    printf("Compounding: %d times per year\n", compounds);
    printf("Final Amount: $%.2lf\n", result);
    printf("Total Interest: $%.2lf\n", result - principal);
    
    return 0;
}