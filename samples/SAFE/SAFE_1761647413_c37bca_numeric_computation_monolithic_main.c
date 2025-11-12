//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 1000

int main() {
    int n;
    double x[MAX_POINTS], y[MAX_POINTS];
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
    double mean_x, mean_y, slope, intercept;
    double numerator, denominator;
    int i;
    
    printf("Enter number of data points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < 2 || n > MAX_POINTS) {
        fprintf(stderr, "Error: Number of points must be between 2 and %d\n", MAX_POINTS);
        return 1;
    }
    
    printf("Enter %d data points (x y):\n", n);
    for (i = 0; i < n; i++) {
        if (scanf("%lf %lf", &x[i], &y[i]) != 2) {
            fprintf(stderr, "Error: Invalid data point\n");
            return 1;
        }
    }
    
    for (i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
    }
    
    mean_x = sum_x / n;
    mean_y = sum_y / n;
    
    numerator = sum_xy - n * mean_x * mean_y;
    denominator = sum_x2 - n * mean_x * mean_x;
    
    if (fabs(denominator) < 1e-10) {
        fprintf(stderr, "Error: Cannot compute slope (denominator too small)\n");
        return 1;
    }
    
    slope = numerator / denominator;
    intercept = mean_y - slope * mean_x;
    
    printf("Linear regression results:\n");
    printf("Slope: %.6f\n", slope);
    printf("Intercept: %.6f\n", intercept);
    printf("Equation: y = %.6fx + %.6f\n", slope, intercept);
    
    double ss_res = 0.0, ss_tot = 0.0;
    for (i = 0; i < n; i++) {
        double y_pred = slope * x[i] + intercept;
        ss_res += (y[i] - y_pred) * (y[i] - y_pred);
        ss_tot += (y[i] - mean_y) * (y[i] - mean_y);
    }
    
    double r_squared;
    if (fabs(ss_tot) < 1e-10) {
        r_squared = 1.0;
    } else {
        r_squared = 1.0 - (ss_res / ss_tot);
    }
    
    printf("R-squared: %.6f\n", r_squared);
    
    return 0;
}