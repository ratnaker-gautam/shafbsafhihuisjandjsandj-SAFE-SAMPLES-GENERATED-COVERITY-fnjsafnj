//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
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
    double variance_x;
    double variance_y;
    double covariance;
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
    
    stats->variance_x = sum_var_x / count;
    stats->variance_y = sum_var_y / count;
    stats->covariance = sum_covar / count;
    
    return stats;
}

void generate_points(struct Point* points, int count) {
    if (points == NULL || count <= 0 || count > MAX_POINTS) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        double base_x = (double)(i % 10);
        double base_y = (double)((i + 5) % 10);
        
        double noise_x = ((double)((i * 7) % 100) - 50.0) / 100.0;
        double noise_y = ((double)((i * 13) % 100) - 50.0) / 100.0;
        
        points[i].x = base_x + noise_x;
        points[i].y = base_y + noise_y;
        
        if (!validate_input(points[i].x, points[i].y)) {
            points[i].x = 0.0;
            points[i].y = 0.0;
        }
    }
}

int main(void) {
    struct Point points_stack[MAX_POINTS];
    int point_count = 50;
    
    if (point_count <= 0 || point_count > MAX_POINTS) {
        fprintf(stderr, "Invalid point count\n");
        return 1;
    }
    
    generate_points(points_stack, point_count);
    
    struct Statistics* heap_stats = compute_statistics(points_stack, point_count);
    if (heap_stats == NULL) {
        fprintf(stderr, "Failed to compute statistics\n");
        return 1;
    }
    
    printf("Statistical Analysis of %d Points:\n", point_count);
    printf("Mean X: %.6f\n", heap_stats->mean_x);
    printf("Mean Y: %.6f\n", heap_stats->mean_y);
    printf("Variance X: %.6f\n", heap_stats->variance_x);
    printf("Variance Y: %.6f\n", heap_stats->variance_y);
    printf("Covariance: %.6f\n", heap_stats->covariance);
    
    double correlation = heap_stats->covariance / 
                        sqrt(heap_stats->variance_x * heap_stats->variance_y);
    if (isfinite(correlation)) {
        printf("Correlation: %.6f\n", correlation);
    } else {
        printf("Correlation: undefined\n");
    }
    
    struct Point* heap_points = malloc(sizeof(struct Point) * point_count);
    if (heap_points != NULL) {
        memcpy(heap_points, points_stack, sizeof(struct Point) * point_count);
        
        double sum_distances = 0.0;
        int valid_pairs = 0;
        
        for (int i = 0; i < point_count - 1; i++) {
            for (int j = i + 1; j < point_count && j < i + 10; j++) {
                double dx = heap_points[i].x - heap_points[j].x;
                double dy = heap_points[i].y - heap_points[j].y;
                double distance = sqrt(dx * dx + dy * dy);
                
                if (isfinite(distance) && distance >= 0.0) {
                    sum_distances += distance;
                    valid_pairs++;
                }
            }
        }
        
        if (valid_pairs > 0) {
            printf("Average distance between points: %.6f\n", sum_distances / valid_pairs);
        } else {
            printf("No valid distance pairs found\n");
        }
        
        free(heap_points