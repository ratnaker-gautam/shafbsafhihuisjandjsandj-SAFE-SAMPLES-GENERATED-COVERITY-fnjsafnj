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
    size_t capacity;
};

int init_point_array(struct PointArray *arr, size_t initial_capacity) {
    if (initial_capacity == 0) return 0;
    arr->points = malloc(initial_capacity * sizeof(struct Point));
    if (arr->points == NULL) return 0;
    arr->count = 0;
    arr->capacity = initial_capacity;
    return 1;
}

void free_point_array(struct PointArray *arr) {
    if (arr->points != NULL) {
        free(arr->points);
        arr->points = NULL;
    }
    arr->count = 0;
    arr->capacity = 0;
}

int add_point(struct PointArray *arr, double x, double y) {
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        if (new_capacity <= arr->capacity) return 0;
        struct Point *new_points = realloc(arr->points, new_capacity * sizeof(struct Point));
        if (new_points == NULL) return 0;
        arr->points = new_points;
        arr->capacity = new_capacity;
    }
    arr->points[arr->count].x = x;
    arr->points[arr->count].y = y;
    arr->count++;
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_input(struct PointArray *arr) {
    printf("Enter number of points (1-%zu): ", (size_t)MAX_POINTS);
    int num_points;
    if (scanf("%d", &num_points) != 1) return 0;
    if (num_points < 1 || num_points > MAX_POINTS) return 0;
    
    for (int i = 0; i < num_points; i++) {
        double x, y;
        printf("Enter point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &x, &y) != 2) return 0;
        if (!add_point(arr, x, y)) return 0;
    }
    return 1;
}

void calculate_distances(struct PointArray *arr) {
    if (arr->count < 2) {
        printf("Need at least 2 points to calculate distances.\n");
        return;
    }
    
    double distances[arr->count - 1];
    for (size_t i = 0; i < arr->count - 1; i++) {
        distances[i] = calculate_distance(arr->points[i], arr->points[i + 1]);
    }
    
    printf("\nDistances between consecutive points:\n");
    double total_distance = 0.0;
    for (size_t i = 0; i < arr->count - 1; i++) {
        printf("Distance from point %zu to point %zu: %.3f\n", 
               i + 1, i + 2, distances[i]);
        total_distance += distances[i];
    }
    
    printf("Total path distance: %.3f\n", total_distance);
    
    if (arr->count > 2) {
        double direct_distance = calculate_distance(arr->points[0], arr->points[arr->count - 1]);
        printf("Direct distance from first to last point: %.3f\n", direct_distance);
    }
}

int main(void) {
    struct PointArray point_array;
    
    if (!init_point_array(&point_array, 10)) {
        fprintf(stderr, "Failed to initialize point array.\n");
        return 1;
    }
    
    printf("Point Distance Calculator\n");
    printf("=========================\n");
    
    if (!read_points_from_input(&point_array)) {
        fprintf(stderr, "Error reading input points.\n");
        free_point_array(&point_array);
        return 1;
    }
    
    calculate_distances(&point_array);
    
    free_point_array(&point_array);
    return 0;
}