//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
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
    double total_distance = 0.0;
    double min_distance = -1.0;
    double max_distance = 0.0;
    
    for (size_t i = 0; i < array->count - 1; i++) {
        double dist = calculate_distance(array->points[i], array->points[i + 1]);
        distances[i] = dist;
        total_distance += dist;
        
        if (min_distance < 0 || dist < min_distance) {
            min_distance = dist;
        }
        if (dist > max_distance) {
            max_distance = dist;
        }
    }
    
    printf("\nDistance Analysis:\n");
    printf("Total path distance: %.6f\n", total_distance);
    printf("Average distance between points: %.6f\n", total_distance / (array->count - 1));
    printf("Minimum distance: %.6f\n", min_distance);
    printf("Maximum distance: %.6f\n", max_distance);
    
    printf("\nIndividual distances:\n");
    for (size_t i = 0; i < array->count - 1; i++) {
        printf("Point %zu to %zu: %.6f\n", i + 1, i + 2, distances[i]);
    }
}

int main(void) {
    struct PointArray point_array = {NULL, 0};
    int stack_buffer[10];
    
    for (int i = 0; i < 10; i++) {
        stack_buffer[i] = i * i;
    }
    
    printf("Point Distance Calculator\n");
    printf("========================\n\n");
    
    if (!read_points_from_user(&point_array)) {
        printf("Error: Invalid input or memory allocation failed.\n");
        return 1;
    }
    
    if (point_array.count > 0) {
        printf("\nPoints entered:\n");
        for (size_t i = 0; i < point_array.count; i++) {
            printf("Point %zu: (%.6f, %.6f)\n", i + 1, point_array.points[i].x, point_array.points[i].y);
        }
        
        calculate_distances(&point_array);
    }
    
    if (point_array.points != NULL) {
        free(point_array.points);
    }
    
    int stack_sum = 0;
    for (int i = 0; i < 10; i++) {
        stack_sum += stack_buffer[i];
    }
    printf("\nStack buffer checksum: %d\n", stack_sum);
    
    return 0;
}