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
};

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 20) return 0;
    
    int dot_count = 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (input[i] < '0' || input[i] > '9') {
            if (i == 0 && input[i] == '-') continue;
            return 0;
        }
    }
    return 1;
}

double safe_strtod(const char *str) {
    char *endptr;
    double result = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') {
        return 0.0;
    }
    return result;
}

struct PointArray *create_point_array(size_t capacity) {
    if (capacity == 0 || capacity > MAX_POINTS) return NULL;
    
    struct PointArray *arr = malloc(sizeof(struct PointArray));
    if (arr == NULL) return NULL;
    
    arr->points = malloc(capacity * sizeof(struct Point));
    if (arr->points == NULL) {
        free(arr);
        return NULL;
    }
    
    arr->count = 0;
    return arr;
}

void destroy_point_array(struct PointArray *arr) {
    if (arr == NULL) return;
    free(arr->points);
    free(arr);
}

int add_point(struct PointArray *arr, double x, double y) {
    if (arr == NULL || arr->count >= MAX_POINTS) return 0;
    
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
    if (arr == NULL || arr->count < 2) return 0.0;
    
    double total = 0.0;
    for (size_t i = 1; i < arr->count; i++) {
        total += calculate_distance(arr->points[i-1], arr->points[i]);
    }
    return total;
}

int main(void) {
    char input_x[32];
    char input_y[32];
    struct PointArray *points = create_point_array(MAX_POINTS);
    
    if (points == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter points (x y). Enter 'done' for both to finish:\n");
    
    while (1) {
        printf("Point %zu: ", points->count + 1);
        
        if (fgets(input_x, sizeof(input_x), stdin) == NULL) break;
        if (fgets(input_y, sizeof(input_y), stdin) == NULL) break;
        
        input_x[strcspn(input_x, "\n")] = '\0';
        input_y[strcspn(input_y, "\n")] = '\0';
        
        if (strcmp(input_x, "done") == 0 && strcmp(input_y, "done") == 0) {
            break;
        }
        
        if (!validate_input(input_x) || !validate_input(input_y)) {
            printf("Invalid input. Please enter numeric values.\n");
            continue;
        }
        
        double x = safe_strtod(input_x);
        double y = safe_strtod(input_y);
        
        if (!add_point(points, x, y)) {
            printf("Cannot add more points. Maximum reached.\n");
            break;
        }
    }
    
    if (points->count > 0) {
        printf("\nPoints entered:\n");
        for (size_t i = 0; i < points->count; i++) {
            printf("Point %zu: (%.2f, %.2f)\n", i+1, points->points[i].x, points->points[i].y);
        }
        
        double total_length = calculate_total_path_length(points);
        printf("\nTotal path length: %.4f\n", total_length);
    } else {
        printf("No points entered.\n");
    }
    
    destroy_point_array(points);
    return 0;
}