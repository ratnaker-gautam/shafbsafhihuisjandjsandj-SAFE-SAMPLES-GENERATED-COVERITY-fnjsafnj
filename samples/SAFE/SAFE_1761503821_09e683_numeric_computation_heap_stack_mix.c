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

int read_points_from_input(struct PointArray *arr) {
    printf("Enter points (x y), one per line. Enter 'q' to finish:\n");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == 'q' || buffer[0] == 'Q') break;
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (!add_point(arr, x, y)) {
                printf("Cannot add more points (maximum %zu)\n", MAX_POINTS);
                break;
            }
        } else {
            printf("Invalid input. Use format: x y\n");
        }
    }
    return arr->count > 0;
}

void calculate_distances(struct PointArray *arr) {
    if (arr->count < 2) {
        printf("Need at least 2 points to calculate distances\n");
        return;
    }
    
    double distances[arr->count - 1];
    for (size_t i = 0; i < arr->count - 1; i++) {
        distances[i] = calculate_distance(arr->points[i], arr->points[i + 1]);
    }
    
    printf("\nDistances between consecutive points:\n");
    double total_distance = 0.0;
    for (size_t i = 0; i < arr->count - 1; i++) {
        printf("Distance from point %zu to %zu: %.6f\n", i, i + 1, distances[i]);
        total_distance += distances[i];
    }
    printf("Total path distance: %.6f\n", total_distance);
    
    if (arr->count >= 3) {
        double min_dist = distances[0];
        double max_dist = distances[0];
        for (size_t i = 1; i < arr->count - 1; i++) {
            if (distances[i] < min_dist) min_dist = distances[i];
            if (distances[i] > max_dist) max_dist = distances[i];
        }
        printf("Minimum distance: %.6f\n", min_dist);
        printf("Maximum distance: %.6f\n", max_dist);
    }
}

int main(void) {
    struct PointArray point_array;
    if (!init_point_array(&point_array, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }
    
    if (!read_points_from_input(&point_array)) {
        printf("No valid points entered\n");
        free_point_array(&point_array);
        return 1;
    }
    
    printf("\nEntered %zu points:\n", point_array.count);
    for (size_t i = 0; i < point_array.count; i++) {
        printf("Point %zu: (%.6f, %.6f)\n", i, point_array.points[i].x, point_array.points[i].y);
    }
    
    calculate_distances(&point_array);
    
    free_point_array(&point_array);
    return 0;
}