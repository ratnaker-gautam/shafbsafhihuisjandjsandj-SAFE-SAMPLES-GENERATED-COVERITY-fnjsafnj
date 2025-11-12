//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
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

int read_points_from_user(struct PointArray *arr) {
    printf("Enter number of points (1-%zu): ", MAX_POINTS);
    size_t num_points;
    if (scanf("%zu", &num_points) != 1) {
        return 0;
    }
    if (num_points < 1 || num_points > MAX_POINTS) {
        return 0;
    }
    
    if (!init_point_array(arr, num_points < 10 ? 10 : num_points)) {
        return 0;
    }
    
    printf("Enter %zu points as x y coordinates:\n", num_points);
    for (size_t i = 0; i < num_points; i++) {
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

void calculate_distances(struct PointArray *arr) {
    if (arr->count < 2) {
        printf("Need at least 2 points to calculate distances.\n");
        return;
    }
    
    double distances[MAX_POINTS][MAX_POINTS];
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            distances[i][j] = calculate_distance(arr->points[i], arr->points[j]);
        }
    }
    
    printf("\nDistance matrix (upper triangle):\n");
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = 0; j < arr->count; j++) {
            if (j > i) {
                printf("%8.3f ", distances[i][j]);
            } else {
                printf("%8s ", "");
            }
        }
        printf("\n");
    }
    
    double min_distance = -1.0;
    size_t min_i = 0, min_j = 0;
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            if (min_distance < 0 || distances[i][j] < min_distance) {
                min_distance = distances[i][j];
                min_i = i;
                min_j = j;
            }
        }
    }
    
    printf("\nClosest points: (%g, %g) and (%g, %g)\n", 
           arr->points[min_i].x, arr->points[min_i].y,
           arr->points[min_j].x, arr->points[min_j].y);
    printf("Distance: %.6f\n", min_distance);
}

int main(void) {
    struct PointArray points;
    
    printf("Point Distance Calculator\n");
    printf("=========================\n");
    
    if (!read_points_from_user(&points)) {
        printf("Error: Invalid input or memory allocation failed.\n");
        return 1;
    }
    
    calculate_distances(&points);
    free_point_array(&points);
    
    return 0;
}