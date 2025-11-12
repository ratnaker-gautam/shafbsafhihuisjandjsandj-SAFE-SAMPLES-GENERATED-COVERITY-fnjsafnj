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

void calculate_distances(struct PointArray *array, double distances[MAX_POINTS][MAX_POINTS]) {
    for (size_t i = 0; i < array->count; i++) {
        for (size_t j = 0; j < array->count; j++) {
            if (i == j) {
                distances[i][j] = 0.0;
            } else {
                distances[i][j] = calculate_distance(array->points[i], array->points[j]);
            }
        }
    }
}

double find_min_distance(double distances[MAX_POINTS][MAX_POINTS], size_t count, size_t *min_i, size_t *min_j) {
    double min_dist = -1.0;
    
    for (size_t i = 0; i < count; i++) {
        for (size_t j = i + 1; j < count; j++) {
            if (min_dist < 0 || distances[i][j] < min_dist) {
                min_dist = distances[i][j];
                *min_i = i;
                *min_j = j;
            }
        }
    }
    
    return min_dist;
}

double find_max_distance(double distances[MAX_POINTS][MAX_POINTS], size_t count, size_t *max_i, size_t *max_j) {
    double max_dist = 0.0;
    
    for (size_t i = 0; i < count; i++) {
        for (size_t j = i + 1; j < count; j++) {
            if (distances[i][j] > max_dist) {
                max_dist = distances[i][j];
                *max_i = i;
                *max_j = j;
            }
        }
    }
    
    return max_dist;
}

int main(void) {
    struct PointArray point_array;
    double distances[MAX_POINTS][MAX_POINTS];
    
    memset(&point_array, 0, sizeof(point_array));
    
    if (!read_points_from_user(&point_array)) {
        printf("Error reading points.\n");
        return 1;
    }
    
    if (point_array.count < 2) {
        printf("Need at least 2 points.\n");
        free(point_array.points);
        return 1;
    }
    
    calculate_distances(&point_array, distances);
    
    size_t min_i, min_j, max_i, max_j;
    double min_dist = find_min_distance(distances, point_array.count, &min_i, &min_j);
    double max_dist = find_max_distance(distances, point_array.count, &max_i, &max_j);
    
    printf("\nResults:\n");
    printf("Closest points: (%.2f, %.2f) and (%.2f, %.2f)\n", 
           point_array.points[min_i].x, point_array.points[min_i].y,
           point_array.points[min_j].x, point_array.points[min_j].y);
    printf("Minimum distance: %.4f\n", min_dist);
    
    printf("Farthest points: (%.2f, %.2f) and (%.2f, %.2f)\n", 
           point_array.points[max_i].x, point_array.points[max_i].y,
           point_array.points[max_j].x, point_array.points[max_j].y);
    printf("Maximum distance: %.4f\n", max_dist);
    
    double total_distance = 0.0;
    size_t pair_count = 0;
    
    for (size_t i = 0; i < point_array.count; i++) {
        for (size_t j = i + 1; j < point_array.count; j++) {
            total_distance += distances[i][j];
            pair_count++;
        }
    }
    
    if (pair_count >