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
    char buffer[256];
    printf("Enter points (x y), one per line. Enter 'done' to finish:\n");
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (strncmp(buffer, "done", 4) == 0) {
            break;
        }
        
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (!add_point(arr, x, y)) {
                printf("Failed to add point. Maximum capacity reached.\n");
                return 0;
            }
            if (arr->count >= MAX_POINTS) {
                printf("Maximum number of points reached.\n");
                break;
            }
        } else {
            printf("Invalid input. Please enter two numbers separated by space.\n");
        }
    }
    return 1;
}

void calculate_distances(struct PointArray *arr) {
    if (arr->count < 2) {
        printf("Need at least 2 points to calculate distances.\n");
        return;
    }
    
    double distances[MAX_POINTS];
    size_t distance_count = 0;
    
    for (size_t i = 0; i < arr->count - 1; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            if (distance_count < MAX_POINTS) {
                distances[distance_count] = calculate_distance(arr->points[i], arr->points[j]);
                distance_count++;
            }
        }
    }
    
    if (distance_count == 0) {
        printf("No distances calculated.\n");
        return;
    }
    
    double min_dist = distances[0];
    double max_dist = distances[0];
    double sum = 0.0;
    
    for (size_t i = 0; i < distance_count; i++) {
        if (distances[i] < min_dist) min_dist = distances[i];
        if (distances[i] > max_dist) max_dist = distances[i];
        sum += distances[i];
    }
    
    double avg_dist = sum / distance_count;
    
    printf("Distance statistics:\n");
    printf("Minimum distance: %.6f\n", min_dist);
    printf("Maximum distance: %.6f\n", max_dist);
    printf("Average distance: %.6f\n", avg_dist);
    printf("Total pairs: %zu\n", distance_count);
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
    
    printf("\nSuccessfully read %zu points.\n", point_array.count);
    
    if (point_array.count > 0) {
        printf("Points entered:\n");
        for (size_t i = 0; i < point_array.count; i++) {
            printf("Point %zu: (%.6f, %.6f)\n", i + 1, point_array.points[i].x, point_array.points[i].y);
        }
        printf("\n");
    }
    
    calculate_distances(&point_array);
    
    free_point_array(&point_array);
    return 0;
}