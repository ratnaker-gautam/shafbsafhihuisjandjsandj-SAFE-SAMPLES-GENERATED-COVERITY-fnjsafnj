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

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    struct PointArray points_heap;
    if (!init_point_array(&points_heap, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    printf("Enter number of points (1-%d): ", MAX_POINTS);
    int num_points;
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Invalid input\n");
        free_point_array(&points_heap);
        return 1;
    }

    if (num_points < 1 || num_points > MAX_POINTS) {
        fprintf(stderr, "Number of points must be between 1 and %d\n", MAX_POINTS);
        free_point_array(&points_heap);
        return 1;
    }

    printf("Enter %d points as x y coordinates:\n", num_points);
    for (int i = 0; i < num_points; i++) {
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            fprintf(stderr, "Invalid point coordinates\n");
            free_point_array(&points_heap);
            return 1;
        }
        if (!add_point(&points_heap, x, y)) {
            fprintf(stderr, "Failed to add point\n");
            free_point_array(&points_heap);
            return 1;
        }
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

    double total_distance = 0.0;
    for (size_t i = 0; i < points_heap.count; i++) {
        double dist = calculate_distance(points_heap.points[i], centroid_stack);
        total_distance += dist;
    }
    double avg_distance = total_distance / points_heap.count;

    printf("Centroid: (%.3f, %.3f)\n", centroid_stack.x, centroid_stack.y);
    printf("Average distance to centroid: %.3f\n", avg_distance);

    struct Point farthest_point_stack;
    double max_distance = -1.0;
    for (size_t i = 0; i < points_heap.count; i++) {
        double dist = calculate_distance(points_heap.points[i], centroid_stack);
        if (dist > max_distance) {
            max_distance = dist;
            farthest_point_stack = points_heap.points[i];
        }
    }

    printf("Farthest point from centroid: (%.3f, %.3f) distance: %.3f\n", 
           farthest_point_stack.x, farthest_point_stack.y, max_distance);

    free_point_array(&points_heap);
    return 0;
}