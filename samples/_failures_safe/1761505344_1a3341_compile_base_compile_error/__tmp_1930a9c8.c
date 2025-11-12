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
        if (new_capacity < arr->capacity) return 0;
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

int read_points_from_input(struct PointArray *arr) {
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

void calculate_statistics(struct PointArray *arr, double *min_dist, double *max_dist, double *avg_dist) {
    if (arr->count < 2) {
        *min_dist = *max_dist = *avg_dist = 0.0;
        return;
    }
    
    double total = 0.0;
    *min_dist = calculate_distance(arr->points[0], arr->points[1]);
    *max_dist = *min_dist;
    total = *min_dist;
    
    size_t pair_count = 1;
    
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            if (i == 0 && j == 1) continue;
            
            double dist = calculate_distance(arr->points[i], arr->points[j]);
            total += dist;
            pair_count++;
            
            if (dist < *min_dist) *min_dist = dist;
            if (dist > *max_dist) *max_dist = dist;
        }
    }
    
    *avg_dist = total / pair_count;
}

int main(void) {
    struct PointArray point_array;
    
    if (!init_point_array(&point_array, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }
    
    printf("Point Distance Statistics Calculator\n");
    printf("====================================\n");
    
    if (!read_points_from_input(&point_array)) {
        fprintf(stderr, "Error reading input points\n");
        free_point_array(&point_array);
        return 1;
    }
    
    if (point_array.count < 2) {
        printf("Need at least 2 points for distance calculations\n");
        free_point_array(&point_array);
        return 1;
    }
    
    double min_dist, max_dist, avg_dist;
    calculate_statistics(&point_array, &min_dist, &max_dist, &avg_dist);
    
    printf("\nDistance Statistics:\n");
    printf("Minimum distance: %.6f\n", min_dist);
    printf("Maximum distance: %.6f\n", max_dist);
    printf("Average distance: %.6f\n", avg_dist);
    
    struct Point centroid;
    centroid.x = 0.0;
    centroid.y = 0.0;
    for (size_t i = 0; i < point_array.count; i++) {
        centroid.x += point_array.points[i].x;
        centroid.y += point_array.points[i].y;
    }
    centroid.x /= point_array.count;
    centroid.y /= point_array.count;
    
    printf("\nCentroid: (%.6f, %.6f)\n", centroid.x, centroid.y);
    
    printf("\nDistances from centroid:\n");
    for (size_t i = 0; i < point_array.count; i++) {
        double dist = calculate_distance(point_array.points[i], centroid);
        printf("Point %zu: %.6f\n