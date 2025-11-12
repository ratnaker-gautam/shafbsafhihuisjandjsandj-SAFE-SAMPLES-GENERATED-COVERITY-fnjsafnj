//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
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
    
    double temp = 1.0 + (rate / compounds_per_year);
    if (temp <= 0) {
        printf("Calculation error\n");
        return 1;
    }
    
    double exponent = (double)years * compounds_per_year;
    if (exponent > 10000) {
        printf("Calculation too large\n");
        return 1;
    }
    
    final_amount = principal * pow(temp, exponent);
    
    if (!isfinite(final_amount)) {
        printf("Calculation overflow\n");
        return 1;
    }
    
    printf("Principal: $%.2lf\n", principal);
    printf("Annual rate: %.2lf%%\n", rate * 100);
    printf("Years: %d\n", years);
    printf("Compounding: %d times per year\n", compounds_per_year);
    printf("Final amount: $%.2lf\n", final_amount);
    
    return 0;
}