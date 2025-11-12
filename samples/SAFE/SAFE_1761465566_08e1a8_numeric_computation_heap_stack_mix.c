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
    if (initial_capacity == 0 || initial_capacity > MAX_POINTS) {
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
        if (new_capacity > MAX_POINTS) {
            new_capacity = MAX_POINTS;
        }
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

double calculate_total_distance(struct PointArray *arr) {
    if (arr->count < 2) {
        return 0.0;
    }
    double total = 0.0;
    for (size_t i = 1; i < arr->count; i++) {
        total += calculate_distance(arr->points[i-1], arr->points[i]);
    }
    return total;
}

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    if (sscanf(buffer, "%lf", value) != 1) {
        return 0;
    }
    return 1;
}

int main() {
    struct PointArray points_heap;
    struct Point points_stack[10];
    size_t stack_count = 0;
    
    printf("Point Distance Calculator\n");
    printf("Enter points (x y). Enter 'q' to finish.\n");
    
    if (!init_point_array(&points_heap, 10)) {
        printf("Error: Failed to initialize point array\n");
        return 1;
    }
    
    while (1) {
        double x, y;
        if (!read_double("Enter x coordinate: ", &x)) {
            break;
        }
        if (!read_double("Enter y coordinate: ", &y)) {
            break;
        }
        
        if (stack_count < 10) {
            points_stack[stack_count].x = x;
            points_stack[stack_count].y = y;
            stack_count++;
        }
        
        if (!add_point(&points_heap, x, y)) {
            printf("Warning: Could not add more points to heap array\n");
            break;
        }
        
        printf("Point added: (%.2f, %.2f)\n", x, y);
        printf("Current point count: %zu\n", points_heap.count);
    }
    
    printf("\nCalculating distances...\n");
    
    double heap_total = calculate_total_distance(&points_heap);
    printf("Total distance (heap points): %.4f\n", heap_total);
    
    double stack_total = 0.0;
    if (stack_count >= 2) {
        for (size_t i = 1; i < stack_count; i++) {
            stack_total += calculate_distance(points_stack[i-1], points_stack[i]);
        }
        printf("Total distance (stack points): %.4f\n", stack_total);
    }
    
    if (points_heap.count > 0) {
        struct Point centroid = {0.0, 0.0};
        for (size_t i = 0; i < points_heap.count; i++) {
            centroid.x += points_heap.points[i].x;
            centroid.y += points_heap.points[i].y;
        }
        centroid.x /= points_heap.count;
        centroid.y /= points_heap.count;
        printf("Centroid of all points: (%.4f, %.4f)\n", centroid.x, centroid.y);
    }
    
    free_point_array(&points_heap);
    return 0;
}