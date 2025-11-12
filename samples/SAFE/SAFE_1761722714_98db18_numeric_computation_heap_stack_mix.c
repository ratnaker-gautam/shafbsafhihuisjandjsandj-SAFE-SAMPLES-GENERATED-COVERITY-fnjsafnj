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
    if (arr == NULL || initial_capacity == 0) return 0;
    arr->points = malloc(initial_capacity * sizeof(struct Point));
    if (arr->points == NULL) return 0;
    arr->count = 0;
    arr->capacity = initial_capacity;
    return 1;
}

int add_point(struct PointArray *arr, double x, double y) {
    if (arr == NULL) return 0;
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        if (new_capacity < arr->capacity) return 0;
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

void free_point_array(struct PointArray *arr) {
    if (arr != NULL) {
        free(arr->points);
        arr->points = NULL;
        arr->count = 0;
        arr->capacity = 0;
    }
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_input(struct PointArray *arr) {
    if (arr == NULL) return 0;
    
    printf("Enter points (x y), one per line. Enter 'done' to finish:\n");
    
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (strncmp(buffer, "done", 4) == 0) break;
        
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (!add_point(arr, x, y)) {
                return 0;
            }
        } else {
            printf("Invalid input. Please enter two numbers or 'done'.\n");
        }
    }
    
    return 1;
}

int main(void) {
    struct PointArray heap_points;
    if (!init_point_array(&heap_points, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }
    
    if (!read_points_from_input(&heap_points)) {
        fprintf(stderr, "Failed to read points\n");
        free_point_array(&heap_points);
        return 1;
    }
    
    if (heap_points.count < 2) {
        printf("Need at least 2 points to calculate distances\n");
        free_point_array(&heap_points);
        return 1;
    }
    
    struct Point stack_points[MAX_POINTS];
    size_t stack_count = heap_points.count < MAX_POINTS ? heap_points.count : MAX_POINTS;
    
    for (size_t i = 0; i < stack_count; i++) {
        stack_points[i] = heap_points.points[i];
    }
    
    printf("\nCalculating distances between consecutive points:\n");
    double total_distance = 0.0;
    
    for (size_t i = 0; i < stack_count - 1; i++) {
        double dist = calculate_distance(stack_points[i], stack_points[i + 1]);
        total_distance += dist;
        printf("Distance between point %zu and point %zu: %.6f\n", i + 1, i + 2, dist);
    }
    
    printf("\nTotal distance along the path: %.6f\n", total_distance);
    
    if (stack_count > 2) {
        double direct_dist = calculate_distance(stack_points[0], stack_points[stack_count - 1]);
        printf("Direct distance between first and last point: %.6f\n", direct_dist);
        printf("Path efficiency ratio: %.6f\n", direct_dist / total_distance);
    }
    
    free_point_array(&heap_points);
    return 0;
}