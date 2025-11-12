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
            printf("Invalid input. Please enter two numbers or 'done'.\n");
        }
    }
    return 1;
}

void calculate_distances(struct PointArray *arr) {
    if (arr == NULL || arr->count < 2) return;
    printf("\nPairwise distances:\n");
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            double dist = calculate_distance(arr->points[i], arr->points[j]);
            printf("Point %zu to Point %zu: %.6f\n", i + 1, j + 1, dist);
        }
    }
}

double calculate_centroid(struct PointArray *arr, char axis) {
    if (arr == NULL || arr->count == 0) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < arr->count; i++) {
        if (axis == 'x') {
            sum += arr->points[i].x;
        } else {
            sum += arr->points[i].y;
        }
    }
    return sum / arr->count;
}

int main(void) {
    struct PointArray point_arr;
    if (!init_point_array(&point_arr, 10)) {
        fprintf(stderr, "Failed to initialize point array.\n");
        return 1;
    }

    if (!read_points_from_user(&point_arr)) {
        free_point_array(&point_arr);
        return 1;
    }

    if (point_arr.count == 0) {
        printf("No points entered.\n");
        free_point_array(&point_arr);
        return 0;
    }

    printf("\nEntered %zu points:\n", point_arr.count);
    for (size_t i = 0; i < point_arr.count; i++) {
        printf("Point %zu: (%.6f, %.6f)\n", i + 1, point_arr.points[i].x, point_arr.points[i].y);
    }

    calculate_distances(&point_arr);

    double centroid_x = calculate_centroid(&point_arr, 'x');
    double centroid_y = calculate_centroid(&point_arr, 'y');
    printf("\nCentroid: (%.6f, %.6f)\n", centroid_x, centroid_y);

    free_point_array(&point_arr);
    return 0;
}