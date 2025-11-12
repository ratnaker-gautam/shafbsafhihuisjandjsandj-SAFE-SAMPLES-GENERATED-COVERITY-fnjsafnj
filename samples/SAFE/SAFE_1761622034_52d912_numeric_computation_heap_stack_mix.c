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

int read_double(const char *prompt, double *value) {
    if (prompt == NULL || value == NULL) return 0;
    printf("%s", prompt);
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) return 0;
    return 1;
}

int main(void) {
    struct PointArray points_heap;
    if (!init_point_array(&points_heap, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    struct Point centroid_stack;
    centroid_stack.x = 0.0;
    centroid_stack.y = 0.0;

    printf("Enter points (x y). Enter 'q' to finish.\n");
    
    double x, y;
    while (points_heap.count < MAX_POINTS) {
        if (!read_double("X: ", &x)) break;
        if (!read_double("Y: ", &y)) break;
        
        if (!add_point(&points_heap, x, y)) {
            fprintf(stderr, "Failed to add point\n");
            free_point_array(&points_heap);
            return 1;
        }
        
        centroid_stack.x += x;
        centroid_stack.y += y;
    }

    if (points_heap.count == 0) {
        printf("No points entered\n");
        free_point_array(&points_heap);
        return 0;
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