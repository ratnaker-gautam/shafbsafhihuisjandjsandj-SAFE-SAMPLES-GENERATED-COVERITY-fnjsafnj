//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 1000

int main() {
    int n;
    printf("Enter number of data points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 2 || n > MAX_POINTS) {
        fprintf(stderr, "Error: Number of points must be between 2 and %d\n", MAX_POINTS);
        return 1;
    }
    
    double x[MAX_POINTS], y[MAX_POINTS];
    printf("Enter %d data points as x y pairs:\n", n);
    
    for (int i = 0; i < n; i++) {
        if (scanf("%lf %lf", &x[i], &y[i]) != 2) {
            fprintf(stderr, "Error: Invalid data point\n");
            return 1;
        }
    }
    
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
    
    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
    }
    
    double denominator = n * sum_x2 - sum_x * sum_x;
    if (fabs(denominator) < 1e-12) {
        fprintf(stderr, "Error: Cannot compute linear regression for this data\n");
        return 1;
    }
    
    double slope = (n * sum_xy - sum_x * sum_y) / denominator;
    double intercept = (sum_y - slope * sum_x) / n;
    
    double sum_residuals = 0.0;
    for (int i = 0; i < n; i++) {
        double predicted = slope * x[i] + intercept;
        double residual = y[i] - predicted;
        sum_residuals += residual * residual;
    }
    
    double r_squared = 1.0 - (sum_residuals / (n * sum_y * sum_y - sum_y * sum_y));
    
    printf("\nLinear Regression Results:\n");
    printf("Slope: %.6f\n", slope);
    printf("Intercept: %.6f\n", intercept);
    printf("R-squared: %.6f\n", r_squared);
    printf("Equation: y = %.6fx + %.6f\n", slope, intercept);
    
    printf("\nPredicted values:\n");
    for (int i = 0; i < n; i++) {
        double predicted = slope * x[i] + intercept;
        printf("x=%.2f, y=%.2f, predicted=%.2f\n", x[i], y[i], predicted);
    }
    
    return 0;
}