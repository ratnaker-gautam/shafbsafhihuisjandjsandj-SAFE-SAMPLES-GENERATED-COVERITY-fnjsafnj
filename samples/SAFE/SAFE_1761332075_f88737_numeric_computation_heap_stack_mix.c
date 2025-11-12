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

double calculate_total_distance(struct PointArray *arr) {
    if (arr->count < 2) return 0.0;
    double total = 0.0;
    for (size_t i = 1; i < arr->count; i++) {
        total += calculate_distance(arr->points[i-1], arr->points[i]);
    }
    return total;
}

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') return 0;
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
            free_point_array(&points);
            return 1;
        }
        
        printf("Point added: (%.2f, %.2f)\n", x, y);
        printf("Current point count: %zu\n", points.count);
    }

    if (points.count > 0) {
        printf("\nCalculating total distance between consecutive points...\n");
        double total_distance = calculate_total_distance(&points);
        printf("Total distance: %.6f\n", total_distance);
        
        if (points.count >= 2) {
            double min_distance = calculate_distance(points.points[0], points.points[1]);
            double max_distance = min_distance;
            
            for (size_t i = 0; i < points.count - 1; i++) {
                for (size_t j = i + 1; j < points.count; j++) {
                    double dist = calculate_distance(points.points[i], points.points[j]);
                    if (dist < min_distance) min_distance = dist;
                    if (dist > max_distance) max_distance = dist;
                }
            }
            
            printf("Minimum distance between any two points: %.6f\n", min_distance);
            printf("Maximum distance between any two points: %.6f\n", max_distance);
        }
    } else {
        printf("No points entered.\n");
    }

    free_point_array(&points);
    return 0;
}