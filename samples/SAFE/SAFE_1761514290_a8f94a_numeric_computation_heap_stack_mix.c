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

double calculate_total_path_length(struct PointArray *arr) {
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
    char buffer[100];
    printf("%s", prompt);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    char *endptr;
    *value = strtod(buffer, &endptr);
    return (endptr != buffer);
}

int main(void) {
    struct PointArray points_heap;
    struct Point points_stack[10];
    size_t stack_count = 0;
    
    printf("Path Length Calculator\n");
    printf("Enter points (x y). Enter 'q' to finish.\n");
    
    if (!init_point_array(&points_heap, 10)) {
        printf("Error: Failed to initialize point array\n");
        return 1;
    }
    
    while (1) {
        double x, y;
        
        if (!read_double("Enter x coordinate (or 'q' to finish): ", &x)) {
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
            printf("Error: Cannot add more points (maximum reached)\n");
            break;
        }
        
        printf("Added point: (%.2f, %.2f)\n", x, y);
    }
    
    if (points_heap.count < 2) {
        printf("Error: At least 2 points are required\n");
        free_point_array(&points_heap);
        return 1;
    }
    
    double total_length = calculate_total_path_length(&points_heap);
    printf("Total path length: %.4f\n", total_length);
    
    double stack_length = 0.0;
    if (stack_count >= 2) {
        for (size_t i = 1; i < stack_count; i++) {
            stack_length += calculate_distance(points_stack[i-1], points_stack[i]);
        }
        printf("Stack points path length: %.4f\n", stack_length);
    }
    
    free_point_array(&points_heap);
    return 0;
}