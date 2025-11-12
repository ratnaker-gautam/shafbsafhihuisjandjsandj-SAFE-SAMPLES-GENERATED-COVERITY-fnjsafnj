//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    double principal, rate, final_amount;
    int years, compounds_per_year;
    
    printf("Enter principal amount: ");
    if (scanf("%lf", &principal) != 1) {
        fprintf(stderr, "Error: Invalid principal amount\n");
        return 1;
    }
    
    if (principal <= 0) {
        fprintf(stderr, "Error: Principal must be positive\n");
        return 1;
    }
    
    printf("Enter annual interest rate (as percentage): ");
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
    if (scanf("%d", &compounds_per_year) != 1) {
        fprintf(stderr, "Error: Invalid compounding frequency\n");
        return 1;
    }
    
    if (compounds_per_year <= 0 || compounds_per_year > 365) {
        fprintf(stderr, "Error: Compounding frequency must be between 1 and 365\n");
        return 1;
    }
    
    rate = rate / 100.0;
    
    double base = 1.0 + (rate / compounds_per_year);
    double exponent = compounds_per_year * years;
    
    if (exponent > 10000) {
        fprintf(stderr, "Error: Calculation would exceed safe limits\n");
        return 1;
    }
    
    final_amount = principal * pow(base, exponent);
    
    if (!isfinite(final_amount)) {
        fprintf(stderr, "Error: Calculation resulted in invalid value\n");
        return 1;
    }
    
    printf("\nCompound Interest Calculation Results:\n");
    printf("Principal: $%.2f\n", principal);
    printf("Annual Rate: %.2f%%\n", rate * 100);
    printf("Years: %d\n", years);
    printf("Compounding: %d times per year\n", compounds_per_year);
    printf("Final Amount: $%.2f\n", final_amount);
    printf("Total Interest: $%.2f\n", final_amount - principal);
    
    return 0;
}