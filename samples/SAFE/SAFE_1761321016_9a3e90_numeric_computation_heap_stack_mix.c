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

void free_point_array(struct PointArray *arr) {
    if (arr != NULL) {
        free(arr->points);
        arr->points = NULL;
        arr->count = 0;
        arr->capacity = 0;
    }
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_user(struct PointArray *arr) {
    if (arr == NULL) return 0;
    printf("Enter points (x y). Enter 0 0 to finish:\n");
    while (1) {
        double x, y;
        int result = scanf("%lf %lf", &x, &y);
        if (result != 2) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter two numbers.\n");
            continue;
        }
        if (x == 0.0 && y == 0.0) break;
        if (!add_point(arr, x, y)) {
            printf("Failed to add point. Maximum capacity reached.\n");
            return 0;
        }
        if (arr->count >= MAX_POINTS) {
            printf("Maximum number of points reached.\n");
            break;
        }
    }
    return 1;
}

void calculate_distances(struct PointArray *arr) {
    if (arr == NULL || arr->count < 2) return;
    struct Point *centroid = malloc(sizeof(struct Point));
    if (centroid == NULL) return;
    centroid->x = 0.0;
    centroid->y = 0.0;
    for (size_t i = 0; i < arr->count; i++) {
        centroid->x += arr->points[i].x;
        centroid->y += arr->points[i].y;
    }
    centroid->x /= arr->count;
    centroid->y /= arr->count;
    printf("Centroid: (%.2f, %.2f)\n", centroid->x, centroid->y);
    printf("Distances from centroid:\n");
    for (size_t i = 0; i < arr->count; i++) {
        double dist = calculate_distance(arr->points[i], *centroid);
        printf("Point %zu: (%.2f, %.2f) -> Distance: %.2f\n", 
               i+1, arr->points[i].x, arr->points[i].y, dist);
    }
    free(centroid);
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
    if (point_array.count == 0) {
        printf("No points entered.\n");
        free_point_array(&point_array);
        return 0;
    }
    calculate_distances(&point_array);
    free_point_array(&point_array);
    return 0;
}