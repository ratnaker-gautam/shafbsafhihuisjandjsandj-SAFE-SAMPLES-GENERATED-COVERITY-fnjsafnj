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
                printf("Failed to add point. Maximum capacity reached.\n");
                return 0;
            }
        } else {
            printf("Invalid input. Please enter two numbers separated by space.\n");
        }
    }
    return 1;
}

void compute_statistics(struct PointArray *arr, double *min_dist, double *max_dist, double *avg_dist) {
    if (arr == NULL || arr->count < 2 || min_dist == NULL || max_dist == NULL || avg_dist == NULL) return;
    *min_dist = -1.0;
    *max_dist = -1.0;
    double total = 0.0;
    size_t pair_count = 0;
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            double dist = calculate_distance(arr->points[i], arr->points[j]);
            if (*min_dist < 0 || dist < *min_dist) *min_dist = dist;
            if (*max_dist < 0 || dist > *max_dist) *max_dist = dist;
            total += dist;
            pair_count++;
        }
    }
    if (pair_count > 0) {
        *avg_dist = total / pair_count;
    } else {
        *min_dist = 0.0;
        *max_dist = 0.0;
        *avg_dist = 0.0;
    }
}

int main() {
    struct PointArray point_array;
    if (!init_point_array(&point_array, 10)) {
        printf("Failed to initialize point array.\n");
        return 1;
    }
    if (!read_points_from_user(&point_array)) {
        free_point_array(&point_array);
        return 1;
    }
    if (point_array.count < 2) {
        printf("At least two points are required for distance calculations.\n");
        free_point_array(&point_array);
        return 1;
    }
    double min_distance, max_distance, avg_distance;
    compute_statistics(&point_array, &min_distance, &max_distance, &avg_distance);
    printf("Distance statistics for %zu points:\n", point_array.count);
    printf("Minimum distance: %.6f\n", min_distance);
    printf("Maximum distance: %.6f\n", max_distance);
    printf("Average distance: %.6f\n", avg_distance);
    free_point_array(&point_array);
    return 0;
}