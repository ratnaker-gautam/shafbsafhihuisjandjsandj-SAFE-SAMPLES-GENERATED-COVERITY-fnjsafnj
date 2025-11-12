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

void free_point_array(struct PointArray *arr) {
    if (arr != NULL) {
        free(arr->points);
        arr->points = NULL;
        arr->count = 0;
        arr->capacity = 0;
    }
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

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_user(struct PointArray *arr) {
    if (arr == NULL) return 0;
    printf("Enter points (x y). Enter 'done' to finish:\n");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (strncmp(buffer, "done", 4) == 0) break;
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (!add_point(arr, x, y)) {
                printf("Failed to add point.\n");
                return 0;
            }
            if (arr->count >= MAX_POINTS) {
                printf("Maximum points reached.\n");
                break;
            }
        } else {
            printf("Invalid input. Use format: x y\n");
        }
    }
    return 1;
}

void calculate_distances(struct PointArray *arr) {
    if (arr == NULL || arr->count < 2) return;
    struct Point *stack_points = malloc(arr->count * sizeof(struct Point));
    if (stack_points == NULL) return;
    memcpy(stack_points, arr->points, arr->count * sizeof(struct Point));
    
    printf("Distance matrix:\n");
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = 0; j < arr->count; j++) {
            if (i != j) {
                double dist = calculate_distance(stack_points[i], stack_points[j]);
                printf("Point %zu to %zu: %.3f\n", i, j, dist);
            }
        }
    }
    free(stack_points);
}

double calculate_centroid(struct PointArray *arr) {
    if (arr == NULL || arr->count == 0) return 0.0;
    double sum_x = 0.0, sum_y = 0.0;
    for (size_t i = 0; i < arr->count; i++) {
        sum_x += arr->points[i].x;
        sum_y += arr->points[i].y;
    }
    double centroid_x = sum_x / arr->count;
    double centroid_y = sum_y / arr->count;
    printf("Centroid: (%.3f, %.3f)\n", centroid_x, centroid_y);
    return sqrt(centroid_x * centroid_x + centroid_y * centroid_y);
}

int main() {
    struct PointArray points;
    if (!init_point_array(&points, 10)) {
        printf("Failed to initialize point array.\n");
        return 1;
    }
    
    if (!read_points_from_user(&points)) {
        printf("Failed to read points.\n");
        free_point_array(&points);
        return 1;
    }
    
    if (points.count == 0) {
        printf("No points entered.\n");
        free_point_array(&points);
        return 1;
    }
    
    printf("Processing %zu points...\n", points.count);
    calculate_distances(&points);
    calculate_centroid(&points);
    
    free_point_array(&points);
    return 0;
}