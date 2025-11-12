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

int main() {
    struct PointArray heap_points;
    if (!init_point_array(&heap_points, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    struct Point stack_points[MAX_POINTS];
    size_t stack_count = 0;

    printf("Enter points (x y), one per line. Enter 'done' to finish:\n");
    
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (strncmp(buffer, "done", 4) == 0) {
            break;
        }
        
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (stack_count < MAX_POINTS) {
                stack_points[stack_count].x = x;
                stack_points[stack_count].y = y;
                stack_count++;
            }
            
            if (!add_point(&heap_points, x, y)) {
                fprintf(stderr, "Failed to add point to heap array\n");
                free_point_array(&heap_points);
                return 1;
            }
        } else {
            printf("Invalid input. Please enter two numbers or 'done'.\n");
        }
    }

    if (heap_points.count < 2) {
        printf("Need at least 2 points for distance calculations.\n");
        free_point_array(&heap_points);
        return 1;
    }

    printf("\nCalculating distances between consecutive points:\n");
    
    double total_distance = 0.0;
    for (size_t i = 0; i < heap_points.count - 1; i++) {
        double dist = calculate_distance(heap_points.points[i], heap_points.points[i + 1]);
        total_distance += dist;
        printf("Distance between (%.2f, %.2f) and (%.2f, %.2f): %.4f\n",
               heap_points.points[i].x, heap_points.points[i].y,
               heap_points.points[i + 1].x, heap_points.points[i + 1].y,
               dist);
    }

    printf("\nTotal path distance: %.4f\n", total_distance);
    
    if (stack_count > 0) {
        printf("\nStack array statistics:\n");
        double min_x = stack_points[0].x;
        double max_x = stack_points[0].x;
        double sum_x = 0.0;
        
        for (size_t i = 0; i < stack_count; i++) {
            if (stack_points[i].x < min_x) min_x = stack_points[i].x;
            if (stack_points[i].x > max_x) max_x = stack_points[i].x;
            sum_x += stack_points[i].x;
        }
        
        printf("X coordinate range: [%.2f, %.2f], Average: %.2f\n",
               min_x, max_x, sum_x / stack_count);
    }

    free_point_array(&heap_points);
    return 0;
}