//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int read_points_from_user(struct PointArray *array) {
    char buffer[256];
    size_t count = 0;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    char *endptr;
    long num_points = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || num_points < 1 || num_points > MAX_POINTS) {
        return 0;
    }
    
    array->points = malloc((size_t)num_points * sizeof(struct Point));
    if (array->points == NULL) {
        return 0;
    }
    
    array->count = (size_t)num_points;
    
    for (size_t i = 0; i < array->count; i++) {
        printf("Enter point %zu (x y): ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            free(array->points);
            return 0;
        }
        
        char *x_end, *y_end;
        double x = strtod(buffer, &x_end);
        double y = strtod(x_end, &y_end);
        
        if (x_end == buffer || y_end == x_end) {
            free(array->points);
            return 0;
        }
        
        array->points[i].x = x;
        array->points[i].y = y;
    }
    
    return 1;
}

void calculate_distances(struct PointArray *array) {
    if (array->count < 2) {
        printf("Need at least 2 points to calculate distances.\n");
        return;
    }
    
    double distances[MAX_POINTS];
    size_t distance_count = 0;
    
    for (size_t i = 0; i < array->count - 1; i++) {
        for (size_t j = i + 1; j < array->count; j++) {
            if (distance_count < MAX_POINTS) {
                distances[distance_count] = calculate_distance(array->points[i], array->points[j]);
                distance_count++;
            }
        }
    }
    
    printf("\nCalculated %zu distances between points:\n", distance_count);
    for (size_t i = 0; i < distance_count; i++) {
        printf("Distance %zu: %.6f\n", i + 1, distances[i]);
    }
    
    if (distance_count > 0) {
        double min_dist = distances[0];
        double max_dist = distances[0];
        double sum = 0.0;
        
        for (size_t i = 0; i < distance_count; i++) {
            if (distances[i] < min_dist) min_dist = distances[i];
            if (distances[i] > max_dist) max_dist = distances[i];
            sum += distances[i];
        }
        
        printf("\nStatistics:\n");
        printf("Minimum distance: %.6f\n", min_dist);
        printf("Maximum distance: %.6f\n", max_dist);
        printf("Average distance: %.6f\n", sum / distance_count);
    }
}

int main(void) {
    struct PointArray point_array = {NULL, 0};
    
    printf("Point Distance Calculator\n");
    printf("========================\n\n");
    
    if (!read_points_from_user(&point_array)) {
        printf("Error: Invalid input or memory allocation failed.\n");
        return 1;
    }
    
    calculate_distances(&point_array);
    
    if (point_array.points != NULL) {
        free(point_array.points);
    }
    
    return 0;
}