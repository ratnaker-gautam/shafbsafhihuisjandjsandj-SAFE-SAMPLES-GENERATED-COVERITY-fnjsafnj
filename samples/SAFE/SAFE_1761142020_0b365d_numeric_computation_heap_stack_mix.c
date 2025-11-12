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

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) return 0;
    return 1;
}

int main() {
    struct PointArray points;
    if (!init_point_array(&points, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    printf("Enter points (x y). Enter 'q' to finish.\n");
    
    while (points.count < MAX_POINTS) {
        double x, y;
        if (!read_double("Enter x: ", &x)) break;
        if (!read_double("Enter y: ", &y)) break;
        
        if (!add_point(&points, x, y)) {
            fprintf(stderr, "Failed to add point\n");
            break;
        }
        
        printf("Point added. Total points: %zu\n", points.count);
    }

    if (points.count < 2) {
        printf("Need at least 2 points for distance calculation\n");
        free_point_array(&points);
        return 1;
    }

    struct Point centroid = {0, 0};
    for (size_t i = 0; i < points.count; i++) {
        centroid.x += points.points[i].x;
        centroid.y += points.points[i].y;
    }
    centroid.x /= points.count;
    centroid.y /= points.count;

    double total_distance = 0.0;
    double min_distance = -1.0;
    double max_distance = 0.0;
    
    for (size_t i = 0; i < points.count; i++) {
        double dist = calculate_distance(points.points[i], centroid);
        total_distance += dist;
        if (min_distance < 0 || dist < min_distance) min_distance = dist;
        if (dist > max_distance) max_distance = dist;
    }
    double avg_distance = total_distance / points.count;

    printf("\nCentroid: (%.3f, %.3f)\n", centroid.x, centroid.y);
    printf("Points: %zu\n", points.count);
    printf("Average distance to centroid: %.3f\n", avg_distance);
    printf("Minimum distance to centroid: %.3f\n", min_distance);
    printf("Maximum distance to centroid: %.3f\n", max_distance);

    free_point_array(&points);
    return 0;
}