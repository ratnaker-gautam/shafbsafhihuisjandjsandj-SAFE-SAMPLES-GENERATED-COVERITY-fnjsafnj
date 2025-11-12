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

void calculate_distances(struct PointArray *array, double *distances) {
    for (size_t i = 0; i < array->count - 1; i++) {
        distances[i] = calculate_distance(array->points[i], array->points[i + 1]);
    }
}

double find_min_distance(double *distances, size_t count) {
    if (count == 0) {
        return 0.0;
    }
    
    double min = distances[0];
    for (size_t i = 1; i < count; i++) {
        if (distances[i] < min) {
            min = distances[i];
        }
    }
    return min;
}

double find_max_distance(double *distances, size_t count) {
    if (count == 0) {
        return 0.0;
    }
    
    double max = distances[0];
    for (size_t i = 1; i < count; i++) {
        if (distances[i] > max) {
            max = distances[i];
        }
    }
    return max;
}

double calculate_average_distance(double *distances, size_t count) {
    if (count == 0) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (size_t i = 0; i < count; i++) {
        sum += distances[i];
    }
    return sum / count;
}

int main(void) {
    struct PointArray points_array;
    double distances_stack[MAX_POINTS];
    
    if (!read_points_from_user(&points_array)) {
        printf("Error: Invalid input or memory allocation failed\n");
        return 1;
    }
    
    if (points_array.count < 2) {
        printf("At least 2 points are required for distance calculations\n");
        free(points_array.points);
        return 1;
    }
    
    size_t distance_count = points_array.count - 1;
    calculate_distances(&points_array, distances_stack);
    
    printf("\nDistance analysis for %zu points:\n", points_array.count);
    printf("Number of segments: %zu\n", distance_count);
    
    for (size_t i = 0; i < distance_count; i++) {
        printf("Distance between point %zu and %zu: %.3f\n", 
               i + 1, i + 2, distances_stack[i]);
    }
    
    double min_dist = find_min_distance(distances_stack, distance_count);
    double max_dist = find_max_distance(distances_stack, distance_count);
    double avg_dist = calculate_average_distance(distances_stack, distance_count);
    
    printf("\nStatistics:\n");
    printf("Minimum distance: %.3f\n", min_dist);
    printf("Maximum distance: %.3f\n", max_dist);
    printf("Average distance: %.3f\n", avg_dist);
    
    free(points_array.points);
    return 0;
}