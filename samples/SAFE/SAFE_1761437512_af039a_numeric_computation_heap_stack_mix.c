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
    struct PointArray points_array;
    double distances[MAX_POINTS - 1];
    double total_distance;
    
    printf("Point Distance Calculator\n");
    printf("========================\n");
    
    if (!read_points_from_user(&points_array)) {
        printf("Error: Invalid input or memory allocation failed.\n");
        return 1;
    }
    
    if (points_array.count < 2) {
        printf("At least 2 points are required to calculate distances.\n");
        free(points_array.points);
        return 1;
    }
    
    calculate_distances(&points_array, distances);
    total_distance = calculate_total_distance(distances, points_array.count - 1);
    
    printf("\nDistance Results:\n");
    for (size_t i = 0; i < points_array.count - 1; i++) {
        printf("Distance between point %zu and %zu: %.3f\n", 
               i + 1, i + 2, distances[i]);
    }
    
    printf("\nTotal distance along the path: %.3f\n", total_distance);
    
    free(points_array.points);
    return 0;
}