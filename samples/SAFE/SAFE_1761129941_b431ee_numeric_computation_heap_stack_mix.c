//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 1000

struct Point {
    double x;
    double y;
};

struct PointArray {
    struct Point *points;
    size_t count;
};

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_user(struct PointArray *arr) {
    size_t count;
    printf("Enter number of points (1-%zu): ", (size_t)MAX_POINTS);
    if (scanf("%zu", &count) != 1) {
        return 0;
    }
    if (count < 1 || count > MAX_POINTS) {
        return 0;
    }
    
    arr->points = malloc(count * sizeof(struct Point));
    if (arr->points == NULL) {
        return 0;
    }
    arr->count = count;
    
    printf("Enter %zu points as x y coordinates:\n", count);
    for (size_t i = 0; i < count; i++) {
        if (scanf("%lf %lf", &arr->points[i].x, &arr->points[i].y) != 2) {
            free(arr->points);
            return 0;
        }
    }
    return 1;
}

void compute_statistics(struct PointArray *arr, double *min_dist, double *max_dist, double *avg_dist) {
    if (arr->count < 2) {
        *min_dist = 0.0;
        *max_dist = 0.0;
        *avg_dist = 0.0;
        return;
    }
    
    double total = 0.0;
    double min = -1.0;
    double max = 0.0;
    size_t pair_count = 0;
    
    for (size_t i = 0; i < arr->count - 1; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            double dist = calculate_distance(arr->points[i], arr->points[j]);
            total += dist;
            pair_count++;
            
            if (min < 0 || dist < min) {
                min = dist;
            }
            if (dist > max) {
                max = dist;
            }
        }
    }
    
    *min_dist = min;
    *max_dist = max;
    *avg_dist = total / pair_count;
}

int main(void) {
    struct PointArray point_array;
    double distances[3];
    
    if (!read_points_from_user(&point_array)) {
        printf("Error: Invalid input or memory allocation failed\n");
        return 1;
    }
    
    if (point_array.count == 1) {
        printf("Only one point provided. No distances to compute.\n");
        free(point_array.points);
        return 0;
    }
    
    compute_statistics(&point_array, &distances[0], &distances[1], &distances[2]);
    
    printf("Distance statistics for %zu points:\n", point_array.count);
    printf("Minimum distance: %.6f\n", distances[0]);
    printf("Maximum distance: %.6f\n", distances[1]);
    printf("Average distance: %.6f\n", distances[2]);
    
    free(point_array.points);
    return 0;
}