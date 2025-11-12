//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_compound_interest(double principal, double rate, int years, int compounds_per_year) {
    if (principal <= 0.0 || rate < 0.0 || years < 0 || compounds_per_year <= 0) {
        return -1.0;
    }
    
    double effective_rate = rate / 100.0;
    double amount = principal * pow(1.0 + effective_rate / compounds_per_year, compounds_per_year * years);
    
    if (isinf(amount) || isnan(amount)) {
        return -1.0;
    }
    
    return amount;
}

int safe_str_to_double(const char *str, double *result) {
    if (str == NULL || result == NULL) return 0;
    
    char *endptr;
    double value = strtod(str, &endptr);
    
    if (endptr == str || *endptr != '\0') return 0;
    if (value == HUGE_VAL || value == -HUGE_VAL || value == 0.0) return 0;
    
    *result = value;
    return 1;
}

int safe_str_to_int(const char *str, int *result) {
    if (str == NULL || result == NULL) return 0;
    
    char *endptr;
    long value = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') return 0;
    if (value > INT_MAX || value < INT_MIN) return 0;
    
    *result = (int)value;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double principal, rate;
    int years, compounds;
    
    printf("Compound Interest Calculator\n");
    printf("Enter principal amount: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!safe_str_to_double(input, &principal) || principal <= 0.0) {
        fprintf(stderr, "Invalid principal amount\n");
        return 1;
    }
    
    printf("Enter annual interest rate (%%): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!safe_str_to_double(input, &rate) || rate < 0.0) {
        fprintf(stderr, "Invalid interest rate\n");
        return 1;
    }
    
    printf("Enter number of years: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!safe_str_to_int(input, &years) || years <= 0) {
        fprintf(stderr, "Invalid number of years\n");
        return 1;
    }
    
    printf("Enter compounding frequency per year: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!safe_str_to_int(input, &compounds) || compounds <= 0) {
        fprintf(stderr, "Invalid compounding frequency\n");
        return 1;
    }
    
    double result = calculate_compound_interest(principal, rate, years, compounds);
    
    if (result < 0.0) {
        fprintf(stderr, "Error calculating compound interest\n");
        return 1;
    }
    
    printf("Principal: $%.2f\n", principal);
    printf("Annual Rate: %.2f%%\n", rate);
    printf("Years: %d\n", years);
    printf("Compounding: %d times per year\n", compounds);
    printf("Future Value: $%.2f\n", result);
    printf("Total Interest: $%.2f\n", result - principal);
    
    return 0;
}