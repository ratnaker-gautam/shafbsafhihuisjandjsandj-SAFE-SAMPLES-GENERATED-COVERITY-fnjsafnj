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

int read_points_from_user(struct PointArray *array) {
    size_t count;
    printf("Enter number of points (1-%zu): ", (size_t)MAX_POINTS);
    if (scanf("%zu", &count) != 1) {
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
    
    printf("Enter %zu points as x y coordinates:\n", count);
    for (size_t i = 0; i < count; i++) {
        if (scanf("%lf %lf", &array->points[i].x, &array->points[i].y) != 2) {
            free(array->points);
            return 0;
        }
    }
    return 1;
}

void compute_distances(struct PointArray *array, double *distances) {
    for (size_t i = 0; i < array->count - 1; i++) {
        distances[i] = calculate_distance(array->points[i], array->points[i + 1]);
    }
}

double find_min_distance(double *distances, size_t count) {
    if (count == 0) return 0.0;
    double min = distances[0];
    for (size_t i = 1; i < count; i++) {
        if (distances[i] < min) {
            min = distances[i];
        }
    }
    return min;
}

double find_max_distance(double *distances, size_t count) {
    if (count == 0) return 0.0;
    double max = distances[0];
    for (size_t i = 1; i < count; i++) {
        if (distances[i] > max) {
            max = distances[i];
        }
    }
    return max;
}

double calculate_average_distance(double *distances, size_t count) {
    if (count == 0) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < count; i++) {
        sum += distances[i];
    }
    return sum / count;
}

int main(void) {
    struct PointArray point_array;
    point_array.points = NULL;
    point_array.count = 0;
    
    if (!read_points_from_user(&point_array)) {
        printf("Error: Invalid input or memory allocation failed.\n");
        return 1;
    }
    
    if (point_array.count < 2) {
        printf("At least 2 points are required for distance calculations.\n");
        free(point_array.points);
        return 1;
    }
    
    size_t distance_count = point_array.count - 1;
    double distances[distance_count];
    
    compute_distances(&point_array, distances);
    
    double min_dist = find_min_distance(distances, distance_count);
    double max_dist = find_max_distance(distances, distance_count);
    double avg_dist = calculate_average_distance(distances, distance_count);
    
    printf("\nDistance Statistics:\n");
    printf("Minimum distance between consecutive points: %.6f\n", min_dist);
    printf("Maximum distance between consecutive points: %.6f\n", max_dist);
    printf("Average distance between consecutive points: %.6f\n", avg_dist);
    
    printf("\nIndividual distances:\n");
    for (size_t i = 0; i < distance_count; i++) {
        printf("Distance between point %zu and %zu: %.6f\n", 
               i, i + 1, distances[i]);
    }
    
    free(point_array.points);
    return 0;
}