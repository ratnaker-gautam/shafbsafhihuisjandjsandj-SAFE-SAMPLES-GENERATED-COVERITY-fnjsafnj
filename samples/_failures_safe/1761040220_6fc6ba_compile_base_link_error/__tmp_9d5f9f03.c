//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 1000

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

int read_points(struct Point *points, int max_count) {
    int count;
    printf("Enter number of points (1-%d): ", max_count);
    if (scanf("%d", &count) != 1) {
        return -1;
    }
    if (count < 1 || count > max_count) {
        return -1;
    }
    
    for (int i = 0; i < count; i++) {
        printf("Enter point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            return -1;
        }
    }
    return count;
}

struct Statistics *compute_statistics(struct Point *points, int count) {
    if (count <= 0 || points == NULL) {
        return NULL;
    }
    
    struct Statistics *stats = malloc(sizeof(struct Statistics));
    if (stats == NULL) {
        return NULL;
    }
    
    double sum_x = 0.0, sum_y = 0.0;
    for (int i = 0; i < count; i++) {
        sum_x += points[i].x;
        sum_y += points[i].y;
    }
    stats->mean_x = sum_x / count;
    stats->mean_y = sum_y / count;
    
    double sum_var_x = 0.0, sum_var_y = 0.0, sum_covar = 0.0;
    for (int i = 0; i < count; i++) {
        double diff_x = points[i].x - stats->mean_x;
        double diff_y = points[i].y - stats->mean_y;
        sum_var_x += diff_x * diff_x;
        sum_var_y += diff_y * diff_y;
        sum_covar += diff_x * diff_y;
    }
    
    if (count > 1) {
        stats->stddev_x = sqrt(sum_var_x / (count - 1));
        stats->stddev_y = sqrt(sum_var_y / (count - 1));
        if (stats->stddev_x > 0.0 && stats->stddev_y > 0.0) {
            stats->correlation = sum_covar / ((count - 1) * stats->stddev_x * stats->stddev_y);
        } else {
            stats->correlation = 0.0;
        }
    } else {
        stats->stddev_x = 0.0;
        stats->stddev_y = 0.0;
        stats->correlation = 0.0;
    }
    
    return stats;
}

void print_statistics(struct Statistics *stats) {
    if (stats == NULL) {
        return;
    }
    printf("Mean X: %.6f\n", stats->mean_x);
    printf("Mean Y: %.6f\n", stats->mean_y);
    printf("StdDev X: %.6f\n", stats->stddev_x);
    printf("StdDev Y: %.6f\n", stats->stddev_y);
    printf("Correlation: %.6f\n", stats->correlation);
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    struct Statistics *stats = compute_statistics(points, count);
    if (stats == NULL) {
        printf("Computation failed\n");
        return 1;
    }
    
    print_statistics(stats);
    free(stats);
    
    return 0;
}