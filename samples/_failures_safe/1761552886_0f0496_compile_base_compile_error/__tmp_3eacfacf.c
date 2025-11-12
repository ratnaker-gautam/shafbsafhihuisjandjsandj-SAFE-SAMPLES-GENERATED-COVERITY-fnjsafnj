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

void compute_statistics(struct PointArray *arr) {
    if (arr == NULL || arr->count < 2) {
        printf("Need at least 2 points for statistics.\n");
        return;
    }
    
    double min_dist = -1.0;
    double max_dist = -1.0;
    double total_dist = 0.0;
    size_t pair_count = 0;
    
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            double dist = calculate_distance(arr->points[i], arr->points[j]);
            if (min_dist < 0 || dist < min_dist) min_dist = dist;
            if (max_dist < 0 || dist > max_dist) max_dist = dist;
            total_dist += dist;
            pair_count++;
        }
    }
    
    printf("Distance statistics for %zu points:\n", arr->count);
    printf("Minimum distance: %.6f\n", min_dist);
    printf("Maximum distance: %.6f\n", max_dist);
    printf("Average distance: %.6f\n", total_dist / pair_count);
}

int main() {
    struct PointArray point_array;
    struct Point static_points[10];
    size_t static_count = 0;
    
    if (!init_point_array(&point_array, 10)) {
        printf("Failed to initialize point array.\n");
        return 1;
    }
    
    printf("First, enter up to 10 points for static array:\n");
    char buffer[256];
    for (int i = 0; i < 10; i++) {
        printf("Point %d (x y): ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            static_points[static_count].x = x;
            static_points[static_count].y = y;
            static_count++;
        } else {
            printf("Invalid input. Moving to dynamic array.\n");
            break;
        }
    }
    
    printf("\nStatic array statistics:\n");
    if (static_count >= 2) {
        double min_dist = -1.0;
        double max_dist = -1.0;
        double total_dist = 0.0;
        size_t pair_count = 0;
        
        for (size_t i = 0; i < static_count; i++) {
            for (size_t j = i + 1; j < static_count; j++) {
                double dist = calculate_distance(static_points[i], static_points[j]);
                if (min_dist < 0 || dist < min_dist) min_dist = dist;
                if (max_dist < 0