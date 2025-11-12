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

int read_points_from_user(struct PointArray *arr) {
    printf("Enter points (x y). Enter 0 0 to finish:\n");
    while (1) {
        double x, y;
        int result = scanf("%lf %lf", &x, &y);
        if (result != 2) {
            printf("Invalid input. Please enter two numbers.\n");
            while (getchar() != '\n');
            continue;
        }
        if (x == 0.0 && y == 0.0) {
            break;
        }
        if (!add_point(arr, x, y)) {
            printf("Failed to add point. Maximum capacity reached.\n");
            return 0;
        }
        if (arr->count >= MAX_POINTS) {
            printf("Maximum number of points reached.\n");
            break;
        }
    }
    return 1;
}

void calculate_statistics(struct PointArray *arr) {
    if (arr->count < 2) {
        printf("Need at least 2 points for statistics.\n");
        return;
    }
    
    double total_distance = 0.0;
    double min_distance = -1.0;
    double max_distance = 0.0;
    size_t pair_count = 0;
    
    for (size_t i = 0; i < arr->count; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            double dist = calculate_distance(arr->points[i], arr->points[j]);
            total_distance += dist;
            pair_count++;
            
            if (min_distance < 0.0 || dist < min_distance) {
                min_distance = dist;
            }
            if (dist > max_distance) {
                max_distance = dist;
            }
        }
    }
    
    if (pair_count > 0) {
        double avg_distance = total_distance / pair_count;
        printf("Distance Statistics:\n");
        printf("Total pairs: %zu\n", pair_count);
        printf("Average distance: %.6f\n", avg_distance);
        printf("Minimum distance: %.6f\n", min_distance);
        printf("Maximum distance: %.6f\n", max_distance);
    }
}

int main(void) {
    struct PointArray point_array;
    
    if (!init_point_array(&point_array, 10)) {
        printf("Failed to initialize point array.\n");
        return 1;
    }
    
    if (!read_points_from_user(&point_array)) {
        free_point_array(&point_array);
        return 1;
    }
    
    if (point_array.count == 0) {
        printf("No points entered.\n");
        free_point_array(&point_array);
        return 0;
    }
    
    printf("\nEntered %zu points:\n", point_array.count);
    for (size_t i = 0; i < point_array.count; i++) {
        printf("Point %zu: (%.6f, %.6f)\n", i + 1, point_array.points[i].x, point_array.points[i].y);
    }
    
    calculate_statistics(&point_array);
    
    free_point_array(&point_array);
    return 0;
}