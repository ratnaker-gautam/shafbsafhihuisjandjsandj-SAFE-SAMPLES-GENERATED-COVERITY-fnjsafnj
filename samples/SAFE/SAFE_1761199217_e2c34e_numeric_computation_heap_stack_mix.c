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
    if (!validate_input(str)) return 0.0;
    char *endptr;
    double result = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') return 0.0;
    if (result == HUGE_VAL || result == -HUGE_VAL) return 0.0;
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
    if (arr != NULL) {
        free(arr->points);
        free(arr);
    }
}

int add_point(struct PointArray *arr, double x, double y) {
    if (arr == NULL || arr->count >= MAX_POINTS) return 0;
    arr->points[arr->count].x = x;
    arr->points[arr->count].y = y;
    arr->count++;
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    if (dx > 1e100 || dy > 1e100) return 0.0;
    return sqrt(dx * dx + dy * dy);
}

double compute_total_distance(struct PointArray *arr) {
    if (arr == NULL || arr->count < 2) return 0.0;
    double total = 0.0;
    for (size_t i = 1; i < arr->count; i++) {
        double dist = calculate_distance(arr->points[i-1], arr->points[i]);
        if (dist == HUGE_VAL || dist == -HUGE_VAL) return 0.0;
        total += dist;
    }
    return total;
}

int main() {
    struct PointArray *point_arr = create_point_array(MAX_POINTS);
    if (point_arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    char input_x[32];
    char input_y[32];
    
    printf("Enter points (x y). Enter 'done' to finish:\n");
    
    while (point_arr->count < MAX_POINTS) {
        printf("Point %zu: ", point_arr->count + 1);
        
        if (scanf("%31s", input_x) != 1) break;
        if (strcmp(input_x, "done") == 0) break;
        
        if (scanf("%31s", input_y) != 1) break;
        if (strcmp(input_y, "done") == 0) break;
        
        double x = safe_strtod(input_x);
        double y = safe_strtod(input_y);
        
        if (!add_point(point_arr, x, y)) {
            fprintf(stderr, "Failed to add point\n");
            break;
        }
    }

    if (point_arr->count < 2) {
        fprintf(stderr, "Need at least 2 points\n");
        destroy_point_array(point_arr);
        return 1;
    }

    double total_dist = compute_total_distance(point_arr);
    if (total_dist == 0.0) {
        fprintf(stderr, "Error in distance calculation\n");
        destroy_point_array(point_arr);
        return 1;
    }

    printf("Total distance through %zu points: %.6f\n", point_arr->count, total_dist);
    
    destroy_point_array(point_arr);
    return 0;
}