//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_POINTS 1000
#define MAX_ITERATIONS 100

struct Point {
    double x;
    double y;
};

struct Statistics {
    double mean_x;
    double mean_y;
    double stddev_x;
    double stddev_y;
    double correlation;
};

int validate_input(double x, double y) {
    if (!isfinite(x) || !isfinite(y)) {
        return 0;
    }
    if (fabs(x) > 1e100 || fabs(y) > 1e100) {
        return 0;
    }
    return 1;
}

struct Statistics* compute_statistics(struct Point* points, int count) {
    if (points == NULL || count <= 0 || count > MAX_POINTS) {
        return NULL;
    }
    
    struct Statistics* stats = malloc(sizeof(struct Statistics));
    if (stats == NULL) {
        return NULL;
    }
    
    double sum_x = 0.0, sum_y = 0.0;
    double sum_x2 = 0.0, sum_y2 = 0.0, sum_xy = 0.0;
    
    for (int i = 0; i < count; i++) {
        if (!validate_input(points[i].x, points[i].y)) {
            free(stats);
            return NULL;
        }
        sum_x += points[i].x;
        sum_y += points[i].y;
        sum_x2 += points[i].x * points[i].x;
        sum_y2 += points[i].y * points[i].y;
        sum_xy += points[i].x * points[i].y;
    }
    
    stats->mean_x = sum_x / count;
    stats->mean_y = sum_y / count;
    
    double var_x = (sum_x2 / count) - (stats->mean_x * stats->mean_x);
    double var_y = (sum_y2 / count) - (stats->mean_y * stats->mean_y);
    
    if (var_x < 0) var_x = 0;
    if (var_y < 0) var_y = 0;
    
    stats->stddev_x = sqrt(var_x);
    stats->stddev_y = sqrt(var_y);
    
    double cov_xy = (sum_xy / count) - (stats->mean_x * stats->mean_y);
    
    if (stats->stddev_x > 0 && stats->stddev_y > 0) {
        stats->correlation = cov_xy / (stats->stddev_x * stats->stddev_y);
        if (stats->correlation < -1.0) stats->correlation = -1.0;
        if (stats->correlation > 1.0) stats->correlation = 1.0;
    } else {
        stats->correlation = 0.0;
    }
    
    return stats;
}

int main() {
    struct Point points[MAX_POINTS];
    int point_count = 0;
    
    printf("Enter number of data points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &point_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (point_count < 1 || point_count > MAX_POINTS) {
        printf("Invalid number of points\n");
        return 1;
    }
    
    printf("Enter %d data points as x y pairs:\n", point_count);
    for (int i = 0; i < point_count; i++) {
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            printf("Invalid input format\n");
            return 1;
        }
        
        if (!validate_input(x, y)) {
            printf("Invalid numeric value\n");
            return 1;
        }
        
        points[i].x = x;
        points[i].y = y;
    }
    
    struct Statistics* stats = compute_statistics(points, point_count);
    if (stats == NULL) {
        printf("Error computing statistics\n");
        return 1;
    }
    
    printf("\nStatistical Analysis Results:\n");
    printf("Mean X: %.6f\n", stats->mean_x);
    printf("Mean Y: %.6f\n", stats->mean_y);
    printf("StdDev X: %.6f\n", stats->stddev_x);
    printf("StdDev Y: %.6f\n", stats->stddev_y);
    printf("Correlation: %.6f\n", stats->correlation);
    
    free(stats);
    
    return 0;
}