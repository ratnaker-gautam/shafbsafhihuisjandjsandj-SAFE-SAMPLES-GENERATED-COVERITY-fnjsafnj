//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    double principal, rate, final_amount;
    int years, compounds_per_year;
    
    printf("Enter principal amount: ");
    if (scanf("%lf", &principal) != 1 || principal <= 0) {
        printf("Invalid principal amount\n");
        return 1;
    }
    
    printf("Enter annual interest rate (as percentage): ");
    if (scanf("%lf", &rate) != 1 || rate <= 0 || rate > 100) {
        printf("Invalid interest rate\n");
        return 1;
    }
    rate = rate / 100.0;
    
    printf("Enter number of years: ");
    if (scanf("%d", &years) != 1 || years <= 0 || years > 100) {
        printf("Invalid number of years\n");
        return 1;
    }
    
    printf("Enter compounding frequency per year: ");
    if (scanf("%d", &compounds_per_year) != 1 || compounds_per_year <= 0 || compounds_per_year > 365) {
        printf("Invalid compounding frequency\n");
        return 1;
    }
    
    if (compounds_per_year > 0 && years > 0 && rate >= 0) {
        double base = 1.0 + (rate / compounds_per_year);
        double exponent = compounds_per_year * years;
        
        if (exponent > 1000.0 || base <= 0.0) {
            printf("Calculation would overflow\n");
            return 1;
        }
        
        final_amount = principal * pow(base, exponent);
        
        if (!isfinite(final_amount)) {
            printf("Calculation resulted in overflow\n");
            return 1;
        }
    } else {
        final_amount = principal;
    }
    
    printf("Principal: $%.2lf\n", principal);
    printf("Annual Rate: %.2lf%%\n", rate * 100);
    printf("Years: %d\n", years);
    printf("Compounding: %d times per year\n", compounds_per_year);
    printf("Final Amount: $%.2lf\n", final_amount);
    
    return 0;
}