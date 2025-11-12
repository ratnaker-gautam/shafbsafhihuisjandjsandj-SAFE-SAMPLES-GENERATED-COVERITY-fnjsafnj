//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 1000

struct Point {
    double x;
    double y;
};

struct PointArray {
    struct Point *points;
    size_t size;
    size_t capacity;
};

int init_point_array(struct PointArray *arr, size_t initial_capacity) {
    if (initial_capacity == 0) return 0;
    arr->points = malloc(initial_capacity * sizeof(struct Point));
    if (!arr->points) return 0;
    arr->size = 0;
    arr->capacity = initial_capacity;
    return 1;
}

void free_point_array(struct PointArray *arr) {
    free(arr->points);
    arr->points = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

int add_point(struct PointArray *arr, double x, double y) {
    if (arr->size >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        if (new_capacity > MAX_POINTS) new_capacity = MAX_POINTS;
        if (new_capacity <= arr->capacity) return 0;
        struct Point *new_points = realloc(arr->points, new_capacity * sizeof(struct Point));
        if (!new_points) return 0;
        arr->points = new_points;
        arr->capacity = new_capacity;
    }
    arr->points[arr->size].x = x;
    arr->points[arr->size].y = y;
    arr->size++;
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

double calculate_total_distance(struct PointArray *arr) {
    if (arr->size < 2) return 0.0;
    double total = 0.0;
    for (size_t i = 1; i < arr->size; i++) {
        total += calculate_distance(arr->points[i-1], arr->points[i]);
    }
    return total;
}

int main() {
    struct PointArray path;
    if (!init_point_array(&path, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    printf("Enter points (x y), one per line. Enter 'q' to finish.\n");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        if (buffer[0] == 'q' || buffer[0] == 'Q') break;
        
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (!add_point(&path, x, y)) {
                fprintf(stderr, "Failed to add point\n");
                free_point_array(&path);
                return 1;
            }
            printf("Added point: (%.2f, %.2f)\n", x, y);
        } else {
            printf("Invalid input. Please enter two numbers separated by space.\n");
        }
    }

    if (path.size > 0) {
        double total_dist = calculate_total_distance(&path);
        printf("Total distance along path: %.4f\n", total_dist);
    } else {
        printf("No points entered.\n");
    }

    free_point_array(&path);
    return 0;
}