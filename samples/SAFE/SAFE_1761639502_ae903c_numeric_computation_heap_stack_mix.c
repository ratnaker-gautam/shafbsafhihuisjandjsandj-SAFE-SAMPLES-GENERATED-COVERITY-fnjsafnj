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
    printf("Enter points (x y), one per line. Enter 'done' to finish:\n");
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

void calculate_distances(struct PointArray *arr, double *distances) {
    if (arr == NULL || distances == NULL || arr->count < 2) return;
    for (size_t i = 0; i < arr->count - 1; i++) {
        distances[i] = calculate_distance(arr->points[i], arr->points[i + 1]);
    }
}

double calculate_total_distance(double *distances, size_t count) {
    double total = 0.0;
    for (size_t i = 0; i < count; i++) {
        total += distances[i];
    }
    return total;
}

int main() {
    struct PointArray points;
    if (!init_point_array(&points, 10)) {
        printf("Failed to initialize point array.\n");
        return 1;
    }

    if (!read_points_from_user(&points)) {
        free_point_array(&points);
        return 1;
    }

    if (points.count < 2) {
        printf("At least 2 points required.\n");
        free_point_array(&points);
        return 1;
    }

    size_t distance_count = points.count - 1;
    double *distances = malloc(distance_count * sizeof(double));
    if (distances == NULL) {
        printf("Memory allocation failed.\n");
        free_point_array(&points);
        return 1;
    }

    calculate_distances(&points, distances);

    printf("\nPoint-to-point distances:\n");
    for (size_t i = 0; i < distance_count; i++) {
        printf("Distance from point %zu to %zu: %.6f\n", i, i + 1, distances[i]);
    }

    double total_distance = calculate_total_distance(distances, distance_count);
    printf("Total path distance: %.6f\n", total_distance);

    free(distances);
    free_point_array(&points);
    return 0;
}