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
    
    if (sscanf(buffer, "%zu", &count) != 1) {
        return 0;
    }
    
    if (count < 1 || count > MAX_POINTS) {
        return 0;
    }
    
    array->points = malloc(count * sizeof(struct Point));
    if (array->points == NULL) {
        return 0;
    }
    array->count = count;
    
    for (size_t i = 0; i < count; i++) {
        printf("Enter point %zu (x y): ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            free(array->points);
            return 0;
        }
        
        if (sscanf(buffer, "%lf %lf", &array->points[i].x, &array->points[i].y) != 2) {
            free(array->points);
            return 0;
        }
    }
    
    return 1;
}

void calculate_distances(struct PointArray *array) {
    if (array->count < 2) {
        printf("Need at least 2 points to calculate distances.\n");
        return;
    }
    
    double distances[MAX_POINTS];
    size_t dist_count = 0;
    
    for (size_t i = 0; i < array->count - 1; i++) {
        for (size_t j = i + 1; j < array->count; j++) {
            if (dist_count < MAX_POINTS) {
                distances[dist_count] = calculate_distance(array->points[i], array->points[j]);
                dist_count++;
            }
        }
    }
    
    double min_dist = distances[0];
    double max_dist = distances[0];
    double sum = 0.0;
    
    for (size_t i = 0; i < dist_count; i++) {
        if (distances[i] < min_dist) min_dist = distances[i];
        if (distances[i] > max_dist) max_dist = distances[i];
        sum += distances[i];
    }
    
    double avg_dist = sum / dist_count;
    
    printf("Distance statistics:\n");
    printf("Minimum distance: %.6f\n", min_dist);
    printf("Maximum distance: %.6f\n", max_dist);
    printf("Average distance: %.6f\n", avg_dist);
    printf("Total pairs analyzed: %zu\n", dist_count);
}

int main(void) {
    struct PointArray point_array;
    point_array.points = NULL;
    point_array.count = 0;
    
    printf("Point Distance Calculator\n");
    printf("========================\n");
    
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