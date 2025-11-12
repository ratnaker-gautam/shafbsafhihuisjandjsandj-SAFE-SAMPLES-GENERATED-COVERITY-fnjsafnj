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

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) {
        return 0;
    }
    return 1;
}

int main() {
    struct PointArray points_heap;
    if (!init_point_array(&points_heap, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    printf("Enter points (x y). Enter 'done' to finish.\n");
    
    while (1) {
        double x, y;
        if (!read_double("Enter x: ", &x)) {
            break;
        }
        if (!read_double("Enter y: ", &y)) {
            break;
        }
        
        if (!add_point(&points_heap, x, y)) {
            fprintf(stderr, "Failed to add point\n");
            free_point_array(&points_heap);
            return 1;
        }
        
        if (points_heap.count >= MAX_POINTS) {
            printf("Maximum points reached\n");
            break;
        }
    }

    if (points_heap.count < 2) {
        printf("Need at least 2 points for distance calculations\n");
        free_point_array(&points_heap);
        return 1;
    }

    struct Point centroid_stack;
    centroid_stack.x = 0.0;
    centroid_stack.y = 0.0;
    
    for (size_t i = 0; i < points_heap.count; i++) {
        centroid_stack.x += points_heap.points[i].x;
        centroid_stack.y += points_heap.points[i].y;
    }
    centroid_stack.x /= points_heap.count;
    centroid_stack.y /= points_heap.count;

    printf("\nCentroid: (%.2f, %.2f)\n", centroid_stack.x, centroid_stack.y);
    printf("\nDistances from centroid:\n");
    
    for (size_t i = 0; i < points_heap.count; i++) {
        double distance = calculate_distance(points_heap.points[i], centroid_stack);
        printf("Point %zu (%.2f, %.2f): distance = %.2f\n", 
               i + 1, points_heap.points[i].x, points_heap.points[i].y, distance);
    }

    double total_distance = 0.0;
    for (size_t i = 0; i < points_heap.count; i++) {
        for (size_t j = i + 1; j < points_heap.count; j++) {
            total_distance += calculate_distance(points_heap.points[i], points_heap.points[j]);
        }
    }
    
    size_t pair_count = (points_heap.count * (points_heap.count - 1)) / 2;
    if (pair_count > 0) {
        double avg_distance = total_distance / pair_count;
        printf("\nAverage distance between all point pairs: %.2f\n", avg_distance);
    }

    free_point_array(&points_heap);
    return 0;
}