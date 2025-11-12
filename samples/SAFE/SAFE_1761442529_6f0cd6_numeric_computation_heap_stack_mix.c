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

int read_points_from_input(struct PointArray *arr) {
    printf("Enter number of points (1-%zu): ", (size_t)MAX_POINTS);
    int num_points;
    if (scanf("%d", &num_points) != 1) {
        return 0;
    }
    if (num_points < 1 || num_points > MAX_POINTS) {
        return 0;
    }
    
    if (!init_point_array(arr, num_points < 10 ? 10 : num_points)) {
        return 0;
    }
    
    printf("Enter %d points as x y coordinates:\n", num_points);
    for (int i = 0; i < num_points; i++) {
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            free_point_array(arr);
            return 0;
        }
        if (!add_point(arr, x, y)) {
            free_point_array(arr);
            return 0;
        }
    }
    return 1;
}

void calculate_statistics(struct PointArray *arr, double *min_dist, double *max_dist, double *avg_dist) {
    if (arr->count < 2) {
        *min_dist = 0.0;
        *max_dist = 0.0;
        *avg_dist = 0.0;
        return;
    }
    
    *min_dist = -1.0;
    *max_dist = -1.0;
    double total_distance = 0.0;
    size_t pair_count = 0;
    
    for (size_t i = 0; i < arr->count - 1; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            double dist = calculate_distance(arr->points[i], arr->points[j]);
            if (*min_dist < 0 || dist < *min_dist) {
                *min_dist = dist;
            }
            if (*max_dist < 0 || dist > *max_dist) {
                *max_dist = dist;
            }
            total_distance += dist;
            pair_count++;
        }
    }
    
    if (pair_count > 0) {
        *avg_dist = total_distance / pair_count;
    } else {
        *avg_dist = 0.0;
    }
}

int main(void) {
    struct PointArray heap_points;
    struct Point stack_points[10];
    size_t stack_count = 0;
    
    printf("Point Distance Statistics Calculator\n");
    
    if (!read_points_from_input(&heap_points)) {
        printf("Error: Invalid input or memory allocation failed\n");
        return 1;
    }
    
    if (heap_points.count > 0 && heap_points.count <= 10) {
        for (size_t i = 0; i < heap_points.count; i++) {
            stack_points[i] = heap_points.points[i];
        }
        stack_count = heap_points.count;
    }
    
    double min_dist, max_dist, avg_dist;
    calculate_statistics(&heap_points, &min_dist, &max_dist, &avg_dist);
    
    printf("\nDistance Statistics:\n");
    printf("Minimum distance: %.6f\n", min_dist);
    printf("Maximum distance: %.6f\n", max_dist);
    printf("Average distance: %.6f\n", avg_dist);
    printf("Total points processed: %zu\n", heap_points.count);
    
    if (stack_count > 0) {
        printf("Points copied to stack: %zu\n", stack_count);
    }
    
    free_point_array(&heap_points);
    return 0;
}