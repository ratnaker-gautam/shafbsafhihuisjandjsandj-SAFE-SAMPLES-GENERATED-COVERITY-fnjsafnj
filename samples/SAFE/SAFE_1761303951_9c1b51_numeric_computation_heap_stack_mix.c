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
    if (arr != NULL && arr->points != NULL) {
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

double calculate_total_distance(struct PointArray *arr) {
    if (arr == NULL || arr->count < 2) return 0.0;
    double total = 0.0;
    for (size_t i = 1; i < arr->count; i++) {
        total += calculate_distance(arr->points[i-1], arr->points[i]);
    }
    return total;
}

int read_points_from_input(struct PointArray *arr) {
    if (arr == NULL) return 0;
    printf("Enter points (x y), one per line. Enter 'q' to finish:\n");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == 'q' || buffer[0] == 'Q') break;
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (!add_point(arr, x, y)) {
                return 0;
            }
            if (arr->count >= MAX_POINTS) break;
        } else {
            printf("Invalid input. Please enter two numbers or 'q' to finish.\n");
        }
    }
    return 1;
}

int main() {
    struct PointArray arr;
    if (!init_point_array(&arr, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    if (!read_points_from_input(&arr)) {
        fprintf(stderr, "Error reading points\n");
        free_point_array(&arr);
        return 1;
    }

    if (arr.count < 2) {
        printf("At least two points are required to calculate distance.\n");
        free_point_array(&arr);
        return 1;
    }

    double total_distance = calculate_total_distance(&arr);
    printf("Total distance along the path: %.6f\n", total_distance);

    struct Point centroid;
    centroid.x = 0.0;
    centroid.y = 0.0;
    for (size_t i = 0; i < arr.count; i++) {
        centroid.x += arr.points[i].x;
        centroid.y += arr.points[i].y;
    }
    centroid.x /= arr.count;
    centroid.y /= arr.count;

    printf("Centroid of points: (%.6f, %.6f)\n", centroid.x, centroid.y);

    double max_distance = 0.0;
    for (size_t i = 0; i < arr.count; i++) {
        double dist = calculate_distance(arr.points[i], centroid);
        if (dist > max_distance) {
            max_distance = dist;
        }
    }
    printf("Maximum distance from centroid: %.6f\n", max_distance);

    free_point_array(&arr);
    return 0;
}