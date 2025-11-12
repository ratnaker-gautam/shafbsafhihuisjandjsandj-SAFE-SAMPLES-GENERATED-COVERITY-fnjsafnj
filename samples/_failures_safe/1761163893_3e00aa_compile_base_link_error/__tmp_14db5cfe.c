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
    if (initial_capacity == 0) {
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

int read_points_from_user(struct PointArray *arr) {
    printf("Enter number of points (1-%zu): ", (size_t)MAX_POINTS);
    int num_points;
    if (scanf("%d", &num_points) != 1) {
        return 0;
    }
    if (num_points < 1 || num_points > MAX_POINTS) {
        return 0;
    }
    
    for (int i = 0; i < num_points; i++) {
        double x, y;
        printf("Enter point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &x, &y) != 2) {
            return 0;
        }
        if (!add_point(arr, x, y)) {
            return 0;
        }
    }
    return 1;
}

void calculate_statistics(struct PointArray *arr) {
    if (arr->count == 0) {
        printf("No points to analyze.\n");
        return;
    }
    
    struct Point centroid_stack;
    centroid_stack.x = 0.0;
    centroid_stack.y = 0.0;
    
    for (size_t i = 0; i < arr->count; i++) {
        centroid_stack.x += arr->points[i].x;
        centroid_stack.y += arr->points[i].y;
    }
    centroid_stack.x /= arr->count;
    centroid_stack.y /= arr->count;
    
    double *distances_heap = malloc(arr->count * sizeof(double));
    if (distances_heap == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    
    double min_distance = -1.0;
    double max_distance = -1.0;
    double total_distance = 0.0;
    
    for (size_t i = 0; i < arr->count; i++) {
        double dist = calculate_distance(arr->points[i], centroid_stack);
        distances_heap[i] = dist;
        total_distance += dist;
        
        if (min_distance < 0 || dist < min_distance) {
            min_distance = dist;
        }
        if (max_distance < 0 || dist > max_distance) {
            max_distance = dist;
        }
    }
    
    double mean_distance = total_distance / arr->count;
    
    double variance = 0.0;
    for (size_t i = 0; i < arr->count; i++) {
        double diff = distances_heap[i] - mean_distance;
        variance += diff * diff;
    }
    variance /= arr->count;
    double std_dev = sqrt(variance);
    
    printf("Centroid: (%.3f, %.3f)\n", centroid_stack.x, centroid_stack.y);
    printf("Distance statistics from centroid:\n");
    printf("  Minimum: %.3f\n", min_distance);
    printf("  Maximum: %.3f\n", max_distance);
    printf("  Mean: %.3f\n", mean_distance);
    printf("  Standard deviation: %.3f\n", std_dev);
    
    free(distances_heap);
}

int main(void) {
    struct PointArray points_heap;
    if (!init_point_array(&points_heap, 10)) {
        printf("Failed to initialize point array.\n");
        return 1;
    }
    
    printf("Point Distance Analyzer\n");
    printf("=======================\n");
    
    if (!read_points_from_user(&points_heap)) {
        printf("Invalid input or memory error.\n");
        free_point_array(&points_heap);
        return 1;
    }
    
    calculate_statistics(&points_heap);
    
    free_point_array(&points_heap);
    return 0;
}