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

double find_statistics(double *distances, size_t count, double *min, double *max, double *avg) {
    if (count == 0) {
        return 0.0;
    }
    
    double sum = 0.0;
    *min = distances[0];
    *max = distances[0];
    
    for (size_t i = 0; i < count; i++) {
        sum += distances[i];
        if (distances[i] < *min) *min = distances[i];
        if (distances[i] > *max) *max = distances[i];
    }
    
    *avg = sum / count;
    
    double variance = 0.0;
    for (size_t i = 0; i < count; i++) {
        double diff = distances[i] - *avg;
        variance += diff * diff;
    }
    
    return sqrt(variance / count);
}

int main(void) {
    struct PointArray points_array;
    double distances_stack[MAX_POINTS];
    
    printf("Point Distance Calculator\n");
    printf("========================\n\n");
    
    if (!read_points_from_user(&points_array)) {
        printf("Error: Invalid input or memory allocation failed.\n");
        return 1;
    }
    
    if (points_array.count < 2) {
        printf("Error: At least 2 points are required.\n");
        free(points_array.points);
        return 1;
    }
    
    size_t distance_count = points_array.count - 1;
    calculate_distances(&points_array, distances_stack);
    
    double min_dist, max_dist, avg_dist;
    double std_dev = find_statistics(distances_stack, distance_count, &min_dist, &max_dist, &avg_dist);
    
    printf("\nDistance Statistics:\n");
    printf("Number of segments: %zu\n", distance_count);
    printf("Minimum distance: %.6f\n", min_dist);
    printf("Maximum distance: %.6f\n", max_dist);
    printf("Average distance: %.6f\n", avg_dist);
    printf("Standard deviation: %.6f\n", std_dev);
    
    printf("\nIndividual distances:\n");
    for (size_t i = 0; i < distance_count; i++) {
        printf("Segment %zu-%zu: %.6f\n", i + 1, i + 2, distances_stack[i]);
    }
    
    free(points_array.points);
    return 0;
}