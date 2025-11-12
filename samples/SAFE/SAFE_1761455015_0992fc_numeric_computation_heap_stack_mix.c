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

void calculate_statistics(struct PointArray *arr) {
    if (arr->count == 0) return;
    
    double total_distance = 0.0;
    double min_distance = -1.0;
    double max_distance = 0.0;
    int pair_count = 0;
    
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            double dist = calculate_distance(arr->points[i], arr->points[j]);
            total_distance += dist;
            pair_count++;
            
            if (min_distance < 0 || dist < min_distance) {
                min_distance = dist;
            }
            if (dist > max_distance) {
                max_distance = dist;
            }
        }
    }
    
    if (pair_count > 0) {
        double avg_distance = total_distance / pair_count;
        printf("Point pair statistics:\n");
        printf("Total pairs: %d\n", pair_count);
        printf("Minimum distance: %.6f\n", min_distance);
        printf("Maximum distance: %.6f\n", max_distance);
        printf("Average distance: %.6f\n", avg_distance);
    } else {
        printf("Only one point provided, no distances to calculate.\n");
    }
}

int main(void) {
    struct PointArray point_array;
    
    if (!init_point_array(&point_array, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }
    
    printf("Point Distance Calculator\n");
    printf("=========================\n");
    
    if (!read_points_from_input(&point_array)) {
        fprintf(stderr, "Error reading input points\n");
        free_point_array(&point_array);
        return 1;
    }
    
    calculate_statistics(&point_array);
    
    free_point_array(&point_array);
    return 0;
}