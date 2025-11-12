//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

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
    if (initial_capacity == 0 || initial_capacity > MAX_POINTS) {
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
        if (new_capacity > MAX_POINTS) {
            new_capacity = MAX_POINTS;
        }
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

double find_min_distance(struct PointArray *arr) {
    if (arr->count < 2) {
        return -1.0;
    }
    double min_dist = calculate_distance(arr->points[0], arr->points[1]);
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            double dist = calculate_distance(arr->points[i], arr->points[j]);
            if (dist < min_dist) {
                min_dist = dist;
            }
        }
    }
    return min_dist;
}

int main() {
    struct PointArray arr;
    if (!init_point_array(&arr, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    printf("Enter number of points (2-%d): ", MAX_POINTS);
    int num_points;
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Invalid input\n");
        free_point_array(&arr);
        return 1;
    }
    if (num_points < 2 || num_points > MAX_POINTS) {
        fprintf(stderr, "Invalid number of points\n");
        free_point_array(&arr);
        return 1;
    }

    printf("Enter %d points as x y coordinates:\n", num_points);
    for (int i = 0; i < num_points; i++) {
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            fprintf(stderr, "Invalid point coordinates\n");
            free_point_array(&arr);
            return 1;
        }
        if (!add_point(&arr, x, y)) {
            fprintf(stderr, "Failed to add point\n");
            free_point_array(&arr);
            return 1;
        }
    }

    double min_dist = find_min_distance(&arr);
    if (min_dist < 0) {
        fprintf(stderr, "Insufficient points for distance calculation\n");
        free_point_array(&arr);
        return 1;
    }

    printf("Minimum distance between any two points: %.6f\n", min_dist);
    free_point_array(&arr);
    return 0;
}