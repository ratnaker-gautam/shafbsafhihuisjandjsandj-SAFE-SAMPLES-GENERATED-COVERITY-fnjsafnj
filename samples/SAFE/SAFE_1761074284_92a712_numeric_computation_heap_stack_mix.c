//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
    if (!arr->points) return 0;
    arr->count = 0;
    arr->capacity = initial_capacity;
    return 1;
}

int add_point(struct PointArray *arr, double x, double y) {
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        if (new_capacity > MAX_POINTS) new_capacity = MAX_POINTS;
        if (new_capacity <= arr->capacity) return 0;
        struct Point *new_points = realloc(arr->points, new_capacity * sizeof(struct Point));
        if (!new_points) return 0;
        arr->points = new_points;
        arr->capacity = new_capacity;
    }
    arr->points[arr->count].x = x;
    arr->points[arr->count].y = y;
    arr->count++;
    return 1;
}

void free_point_array(struct PointArray *arr) {
    free(arr->points);
    arr->points = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_user(struct PointArray *arr) {
    printf("Enter points (x y), one per line. Enter 'q' to finish:\n");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        if (buffer[0] == 'q' || buffer[0] == 'Q') break;
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (!add_point(arr, x, y)) {
                printf("Cannot add more points (maximum %zu)\n", MAX_POINTS);
                break;
            }
        } else {
            printf("Invalid input. Use format: x y\n");
        }
    }
    return arr->count > 0;
}

int main(void) {
    struct PointArray heap_points;
    if (!init_point_array(&heap_points, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    if (!read_points_from_user(&heap_points)) {
        printf("No valid points entered\n");
        free_point_array(&heap_points);
        return 1;
    }

    struct Point stack_points[MAX_POINTS];
    size_t stack_count = 0;

    for (size_t i = 0; i < heap_points.count && stack_count < MAX_POINTS; i++) {
        stack_points[stack_count] = heap_points.points[i];
        stack_count++;
    }

    if (stack_count < 2) {
        printf("Need at least 2 points for distance calculation\n");
        free_point_array(&heap_points);
        return 1;
    }

    double total_distance = 0.0;
    for (size_t i = 1; i < stack_count; i++) {
        double dist = calculate_distance(stack_points[i-1], stack_points[i]);
        total_distance += dist;
        printf("Distance between point %zu and %zu: %.6f\n", i-1, i, dist);
    }

    printf("Total distance along path: %.6f\n", total_distance);

    struct Point centroid = {0.0, 0.0};
    for (size_t i = 0; i < stack_count; i++) {
        centroid.x += stack_points[i].x;
        centroid.y += stack_points[i].y;
    }
    centroid.x /= stack_count;
    centroid.y /= stack_count;

    printf("Centroid of all points: (%.6f, %.6f)\n", centroid.x, centroid.y);

    free_point_array(&heap_points);
    return 0;
}