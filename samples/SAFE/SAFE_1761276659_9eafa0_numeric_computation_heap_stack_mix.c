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
        if (new_capacity > MAX_POINTS) new_capacity = MAX_POINTS;
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
    printf("Enter points (x y). Enter 0 0 to finish:\n");
    while (1) {
        double x, y;
        int result = scanf("%lf %lf", &x, &y);
        if (result != 2) {
            printf("Invalid input. Please enter two numbers.\n");
            while (getchar() != '\n');
            continue;
        }
        if (x == 0.0 && y == 0.0) break;
        if (!add_point(arr, x, y)) {
            printf("Cannot add more points. Maximum reached.\n");
            break;
        }
        if (arr->count >= MAX_POINTS) break;
    }
    return arr->count > 0;
}

void calculate_distances(struct PointArray *arr) {
    if (arr->count < 2) {
        printf("Need at least 2 points to calculate distances.\n");
        return;
    }
    double distances[MAX_POINTS];
    size_t dist_count = 0;
    for (size_t i = 0; i < arr->count - 1; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            if (dist_count >= MAX_POINTS) break;
            distances[dist_count] = calculate_distance(arr->points[i], arr->points[j]);
            dist_count++;
        }
    }
    double min_dist = distances[0];
    double max_dist = distances[0];
    double sum = 0.0;
    for (size_t i = 0; i < dist_count; i++) {
        if (distances[i] < min_dist) min_dist = distances[i];
        if (distances[i] > max_dist) max_dist = distances[i];
        sum += distances[i];
    }
    double avg_dist = sum / dist_count;
    printf("Distance statistics:\n");
    printf("Minimum distance: %.6f\n", min_dist);
    printf("Maximum distance: %.6f\n", max_dist);
    printf("Average distance: %.6f\n", avg_dist);
}

int main(void) {
    struct PointArray points_heap;
    if (!init_point_array(&points_heap, 10)) {
        fprintf(stderr, "Failed to initialize point array.\n");
        return 1;
    }
    if (!read_points_from_user(&points_heap)) {
        printf("No valid points entered.\n");
        free_point_array(&points_heap);
        return 1;
    }
    printf("Entered %zu points:\n", points_heap.count);
    for (size_t i = 0; i < points_heap.count; i++) {
        printf("Point %zu: (%.6f, %.6f)\n", i + 1, points_heap.points[i].x, points_heap.points[i].y);
    }
    calculate_distances(&points_heap);
    free_point_array(&points_heap);
    return 0;
}