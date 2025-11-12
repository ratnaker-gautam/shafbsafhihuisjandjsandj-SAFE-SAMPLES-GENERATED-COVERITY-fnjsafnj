//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_POINTS 1000
#define MAX_ITERATIONS 100

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

void free_point_array(struct PointArray *arr) {
    if (arr->points) free(arr->points);
    arr->points = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

int add_point(struct PointArray *arr, double x, double y) {
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
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

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_user(struct PointArray *arr) {
    printf("Enter number of points (1-%zu): ", (size_t)MAX_POINTS);
    int num_points;
    if (scanf("%d", &num_points) != 1) return 0;
    if (num_points < 1 || num_points > MAX_POINTS) return 0;
    
    for (int i = 0; i < num_points; i++) {
        double x, y;
        printf("Enter point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &x, &y) != 2) return 0;
        if (!add_point(arr, x, y)) return 0;
    }
    return 1;
}

void calculate_centroid(struct PointArray *arr, struct Point *centroid) {
    double sum_x = 0.0, sum_y = 0.0;
    for (size_t i = 0; i < arr->count; i++) {
        sum_x += arr->points[i].x;
        sum_y += arr->points[i].y;
    }
    centroid->x = sum_x / arr->count;
    centroid->y = sum_y / arr->count;
}

double calculate_total_distance_to_centroid(struct PointArray *arr, struct Point centroid) {
    double total_distance = 0.0;
    for (size_t i = 0; i < arr->count; i++) {
        total_distance += calculate_distance(arr->points[i], centroid);
    }
    return total_distance;
}

int main(void) {
    struct PointArray points;
    if (!init_point_array(&points, 10)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Point Distance Calculator\n");
    if (!read_points_from_user(&points)) {
        fprintf(stderr, "Invalid input or memory error\n");
        free_point_array(&points);
        return 1;
    }
    
    if (points.count == 0) {
        fprintf(stderr, "No points entered\n");
        free_point_array(&points);
        return 1;
    }
    
    struct Point centroid;
    calculate_centroid(&points, &centroid);
    
    double total_distance = calculate_total_distance_to_centroid(&points, centroid);
    
    printf("\nResults:\n");
    printf("Centroid: (%.3f, %.3f)\n", centroid.x, centroid.y);
    printf("Total distance to centroid: %.3f\n", total_distance);
    printf("Average distance to centroid: %.3f\n", total_distance / points.count);
    
    free_point_array(&points);
    return 0;
}