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
    if (arr == NULL || initial_capacity == 0 || initial_capacity > MAX_POINTS) {
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
    if (arr != NULL && arr->points != NULL) {
        free(arr->points);
        arr->points = NULL;
        arr->count = 0;
        arr->capacity = 0;
    }
}

int add_point(struct PointArray *arr, double x, double y) {
    if (arr == NULL || arr->points == NULL) {
        return 0;
    }
    
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
    
    if (arr->count < arr->capacity) {
        arr->points[arr->count].x = x;
        arr->points[arr->count].y = y;
        arr->count++;
        return 1;
    }
    
    return 0;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

double calculate_total_distance(struct PointArray *arr) {
    if (arr == NULL || arr->points == NULL || arr->count < 2) {
        return 0.0;
    }
    
    double total = 0.0;
    for (size_t i = 1; i < arr->count; i++) {
        total += calculate_distance(arr->points[i-1], arr->points[i]);
    }
    return total;
}

int main() {
    struct PointArray path;
    struct Point stack_points[10];
    size_t stack_count = 0;
    
    printf("Enter number of points to process (2-100): ");
    int num_points;
    if (scanf("%d", &num_points) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (num_points < 2 || num_points > 100) {
        printf("Number of points must be between 2 and 100\n");
        return 1;
    }
    
    if (!init_point_array(&path, 10)) {
        printf("Failed to initialize point array\n");
        return 1;
    }
    
    printf("Enter %d points (x y coordinates):\n", num_points);
    
    for (int i = 0; i < num_points; i++) {
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            printf("Invalid point coordinates\n");
            free_point_array(&path);
            return 1;
        }
        
        if (i < 10) {
            stack_points[stack_count].x = x;
            stack_points[stack_count].y = y;
            stack_count++;
        }
        
        if (!add_point(&path, x, y)) {
            printf("Failed to add point to array\n");
            free_point_array(&path);
            return 1;
        }
    }
    
    double total_distance = calculate_total_distance(&path);
    printf("Total distance along path: %.6f\n", total_distance);
    
    if (stack_count >= 2) {
        double stack_distance = calculate_distance(stack_points[0], stack_points[stack_count-1]);
        printf("Distance between first and last stack points: %.6f\n", stack_distance);
    }
    
    free_point_array(&path);
    return 0;
}