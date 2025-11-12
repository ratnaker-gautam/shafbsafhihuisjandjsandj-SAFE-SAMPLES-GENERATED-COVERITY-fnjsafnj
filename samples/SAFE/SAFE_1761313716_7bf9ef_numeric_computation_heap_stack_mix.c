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
    size_t count;
    
    printf("Enter number of points (1-%zu): ", (size_t)MAX_POINTS);
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
    
    double distances[MAX_POINTS - 1];
    
    for (size_t i = 0; i < array->count - 1; i++) {
        distances[i] = calculate_distance(array->points[i], array->points[i + 1]);
    }
    
    printf("\nDistances between consecutive points:\n");
    for (size_t i = 0; i < array->count - 1; i++) {
        printf("Distance between point %zu and %zu: %.6f\n", 
               i + 1, i + 2, distances[i]);
    }
    
    double total_distance = 0.0;
    for (size_t i = 0; i < array->count - 1; i++) {
        total_distance += distances[i];
    }
    
    printf("Total path distance: %.6f\n", total_distance);
}

int main(void) {
    struct PointArray point_array = {NULL, 0};
    struct PointArray stack_array;
    struct Point stack_points[10];
    
    stack_array.points = stack_points;
    stack_array.count = 3;
    
    stack_points[0].x = 0.0;
    stack_points[0].y = 0.0;
    stack_points[1].x = 3.0;
    stack_points[1].y = 4.0;
    stack_points[2].x = 6.0;
    stack_points[2].y = 8.0;
    
    printf("Default triangle points on stack:\n");
    for (size_t i = 0; i < stack_array.count; i++) {
        printf("Point %zu: (%.1f, %.1f)\n", i + 1, stack_points[i].x, stack_points[i].y);
    }
    calculate_distances(&stack_array);
    
    printf("\nNow enter your own points:\n");
    if (!read_points_from_user(&point_array)) {
        printf("Error reading points. Using default points.\n");
        calculate_distances(&stack_array);
        return 1;
    }
    
    calculate_distances(&point_array);
    
    if (point_array.points != NULL) {
        free(point_array.points);
    }
    
    return 0;
}