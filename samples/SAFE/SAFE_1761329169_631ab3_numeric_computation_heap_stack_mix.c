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
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_user(struct PointArray *array) {
    char buffer[256];
    size_t count;
    
    printf("Enter number of points (1-%zu): ", MAX_POINTS);
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

void calculate_distances(struct PointArray *array, double *distances) {
    for (size_t i = 0; i < array->count - 1; i++) {
        distances[i] = calculate_distance(array->points[i], array->points[i + 1]);
    }
}

double calculate_total_distance(double *distances, size_t count) {
    double total = 0.0;
    for (size_t i = 0; i < count; i++) {
        total += distances[i];
    }
    return total;
}

int main(void) {
    struct PointArray point_array;
    double distances_stack[MAX_POINTS - 1];
    
    printf("Point Distance Calculator\n");
    printf("========================\n");
    
    if (!read_points_from_user(&point_array)) {
        printf("Error: Invalid input or memory allocation failed\n");
        return 1;
    }
    
    if (point_array.count < 2) {
        printf("At least 2 points are required to calculate distances\n");
        free(point_array.points);
        return 1;
    }
    
    calculate_distances(&point_array, distances_stack);
    
    size_t distance_count = point_array.count - 1;
    double total_distance = calculate_total_distance(distances_stack, distance_count);
    
    printf("\nDistance Results:\n");
    for (size_t i = 0; i < distance_count; i++) {
        printf("Distance between point %zu and %zu: %.2f\n", 
               i + 1, i + 2, distances_stack[i]);
    }
    
    printf("\nTotal distance: %.2f\n", total_distance);
    
    double *normalized_distances = malloc(distance_count * sizeof(double));
    if (normalized_distances != NULL) {
        for (size_t i = 0; i < distance_count; i++) {
            normalized_distances[i] = distances_stack[i] / total_distance;
        }
        
        printf("\nNormalized distances (as fraction of total):\n");
        for (size_t i = 0; i < distance_count; i++) {
            printf("Segment %zu: %.4f\n", i + 1, normalized_distances[i]);
        }
        
        free(normalized_distances);
    }
    
    free(point_array.points);
    return 0;
}