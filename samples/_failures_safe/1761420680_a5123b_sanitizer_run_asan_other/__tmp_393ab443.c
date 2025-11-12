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
        printf("Distance from point %zu to point %zu: %.6f\n", 
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
    struct PointArray *heap_array = malloc(sizeof(struct PointArray));
    
    if (heap_array == NULL) {
        return 1;
    }
    
    heap_array->points = NULL;
    heap_array->count = 0;
    
    printf("Point Distance Calculator\n");
    printf("=========================\n\n");
    
    if (!read_points_from_user(heap_array)) {
        printf("Error reading points. Please check your input.\n");
        free(heap_array->points);
        free(heap_array);
        return 1;
    }
    
    struct PointArray stack_array;
    stack_array.points = malloc(heap_array->count * sizeof(struct Point));
    if (stack_array.points == NULL) {
        free(heap_array->points);
        free(heap_array);
        return 1;
    }
    
    stack_array.count = heap_array->count;
    memcpy(stack_array.points, heap_array->points, heap_array->count * sizeof(struct Point));
    
    calculate_distances(&stack_array);
    
    free(stack_array.points);
    free(heap_array->points);
    free(heap_array);
    
    return 0;
}