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
    if (initial_capacity == 0) {
        return 0;
    }
    arr->points = malloc(initial_capacity * sizeof(struct Point));
    if (arr->points == NULL) {
        return 0;
    }
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
        if (new_capacity <= arr->capacity) {
            return 0;
        }
        struct Point *new_points = realloc(arr->points, new_capacity * sizeof(struct Point));
        if (new_points == NULL) {
            return 0;
        }
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

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) {
        return 0;
    }
    return 1;
}

int main() {
    struct PointArray heap_points;
    if (!init_point_array(&heap_points, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    struct Point stack_points[MAX_POINTS];
    size_t stack_count = 0;

    printf("Enter points (x y). Enter '0 0' to finish:\n");

    while (1) {
        double x, y;
        if (!read_double("X: ", &x) || !read_double("Y: ", &y)) {
            fprintf(stderr, "Invalid input\n");
            free_point_array(&heap_points);
            return 1;
        }

        if (x == 0.0 && y == 0.0) {
            break;
        }

        if (stack_count < MAX_POINTS) {
            stack_points[stack_count].x = x;
            stack_points[stack_count].y = y;
            stack_count++;
        }

        if (!add_point(&heap_points, x, y)) {
            fprintf(stderr, "Failed to add point to heap array\n");
            free_point_array(&heap_points);
            return 1;
        }
    }

    if (heap_points.count < 2) {
        printf("Need at least 2 points\n");
        free_point_array(&heap_points);
        return 1;
    }

    double total_distance = 0.0;
    for (size_t i = 1; i < heap_points.count; i++) {
        double dist = calculate_distance(heap_points.points[i-1], heap_points.points[i]);
        total_distance += dist;
    }

    printf("Total path distance (heap): %.6f\n", total_distance);

    total_distance = 0.0;
    for (size_t i = 1; i < stack_count; i++) {
        double dist = calculate_distance(stack_points[i-1], stack_points[i]);
        total_distance += dist;
    }

    printf("Total path distance (stack): %.6f\n", total_distance);

    struct Point centroid;
    centroid.x = 0.0;
    centroid.y = 0.0;
    for (size_t i = 0; i < heap_points.count; i++) {
        centroid.x += heap_points.points[i].x;
        centroid.y += heap_points.points[i].y;
    }
    centroid.x /= heap_points.count;
    centroid.y /= heap_points.count;

    printf("Centroid: (%.6f, %.6f)\n", centroid.x, centroid.y);

    free_point_array(&heap_points);
    return 0;
}