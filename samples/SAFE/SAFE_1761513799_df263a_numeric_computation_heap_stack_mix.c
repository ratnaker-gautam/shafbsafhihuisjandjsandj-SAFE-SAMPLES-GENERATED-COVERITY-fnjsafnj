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
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    return 1;
}

int main(void) {
    struct PointArray heap_points;
    if (!init_point_array(&heap_points, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    printf("Enter points (x y). Enter 'q' to finish.\n");
    
    while (1) {
        double x, y;
        if (!read_double("Enter x: ", &x)) {
            break;
        }
        if (!read_double("Enter y: ", &y)) {
            break;
        }
        
        if (!add_point(&heap_points, x, y)) {
            fprintf(stderr, "Failed to add point\n");
            free_point_array(&heap_points);
            return 1;
        }
        
        if (heap_points.count >= MAX_POINTS) {
            printf("Maximum number of points reached\n");
            break;
        }
    }

    if (heap_points.count < 2) {
        printf("Need at least 2 points for distance calculations\n");
        free_point_array(&heap_points);
        return 1;
    }

    struct Point stack_points[MAX_POINTS];
    size_t stack_count = 0;
    
    for (size_t i = 0; i < heap_points.count && stack_count < MAX_POINTS; i++) {
        stack_points[stack_count] = heap_points.points[i];
        stack_count++;
    }

    printf("\nCalculating distances between consecutive points:\n");
    double total_distance = 0.0;
    for (size_t i = 0; i < stack_count - 1; i++) {
        double distance = calculate_distance(stack_points[i], stack_points[i + 1]);
        total_distance += distance;
        printf("Distance between point %zu and %zu: %.6f\n", i + 1, i + 2, distance);
    }

    printf("Total distance: %.6f\n", total_distance);
    
    if (stack_count > 0) {
        double centroid_x = 0.0, centroid_y = 0.0;
        for (size_t i = 0; i < stack_count; i++) {
            centroid_x += stack_points[i].x;
            centroid_y += stack_points[i].y;
        }
        centroid_x /= stack_count;
        centroid_y /= stack_count;
        printf("Centroid: (%.6f, %.6f)\n", centroid_x, centroid_y);
    }

    free_point_array(&heap_points);
    return 0;
}