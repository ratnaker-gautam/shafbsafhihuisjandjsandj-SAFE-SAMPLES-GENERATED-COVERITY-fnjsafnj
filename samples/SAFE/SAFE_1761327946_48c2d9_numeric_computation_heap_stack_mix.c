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
    struct PointArray point_array;
    struct Point stack_points[10];
    size_t stack_count = 0;
    
    if (!init_point_array(&point_array, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }
    
    printf("Enter points (x y), one per line. Enter 'q' to finish:\n");
    
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == 'q' || buffer[0] == 'Q') {
            break;
        }
        
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (stack_count < 10) {
                stack_points[stack_count].x = x;
                stack_points[stack_count].y = y;
                stack_count++;
            }
            
            if (!add_point(&point_array, x, y)) {
                fprintf(stderr, "Failed to add point to array\n");
                break;
            }
        } else {
            fprintf(stderr, "Invalid input format. Use: x y\n");
        }
    }
    
    if (point_array.count > 0) {
        printf("\nPoints entered:\n");
        for (size_t i = 0; i < point_array.count; i++) {
            printf("Point %zu: (%.2f, %.2f)\n", i+1, point_array.points[i].x, point_array.points[i].y);
        }
        
        double total_dist = calculate_total_distance(&point_array);
        printf("\nTotal distance between consecutive points: %.4f\n", total_dist);
        
        if (stack_count > 0) {
            printf("\nFirst %zu points from stack:\n", stack_count);
            for (size_t i = 0; i < stack_count; i++) {
                printf("Stack point %zu: (%.2f, %.2f)\n", i+1, stack_points[i].x, stack_points[i].y);
            }
        }
    } else {
        printf("No valid points entered.\n");
    }
    
    free_point_array(&point_array);
    return 0;
}