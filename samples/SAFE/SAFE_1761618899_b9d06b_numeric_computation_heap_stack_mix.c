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
            if (arr->count >= MAX_POINTS) break;
        } else {
            printf("Invalid input. Please enter two numbers or 'done'.\n");
        }
    }
    
    return arr->count > 0;
}

int main(void) {
    struct PointArray points_heap;
    if (!init_point_array(&points_heap, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }
    
    if (!read_points_from_input(&points_heap)) {
        fprintf(stderr, "No valid points entered\n");
        free_point_array(&points_heap);
        return 1;
    }
    
    if (points_heap.count < 2) {
        fprintf(stderr, "At least 2 points required\n");
        free_point_array(&points_heap);
        return 1;
    }
    
    struct Point centroid_stack;
    centroid_stack.x = 0.0;
    centroid_stack.y = 0.0;
    
    for (size_t i = 0; i < points_heap.count; i++) {
        centroid_stack.x += points_heap.points[i].x;
        centroid_stack.y += points_heap.points[i].y;
    }
    
    centroid_stack.x /= points_heap.count;
    centroid_stack.y /= points_heap.count;
    
    printf("Centroid: (%.3f, %.3f)\n", centroid_stack.x, centroid_stack.y);
    
    double total_distance = 0.0;
    for (size_t i = 0; i < points_heap.count; i++) {
        double dist = calculate_distance(points_heap.points[i], centroid_stack);
        total_distance += dist;
        printf("Point %zu: (%.3f, %.3f) Distance to centroid: %.3f\n", 
               i + 1, points_heap.points[i].x, points_heap.points[i].y, dist);
    }
    
    double avg_distance = total_distance / points_heap.count;
    printf("Average distance to centroid: %.3f\n", avg_distance);
    
    free_point_array(&points_heap);
    return 0;
}