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

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_user(struct PointArray *array) {
    char buffer[256];
    size_t count = 0;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    char *endptr;
    long num_points = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || num_points <= 0 || num_points > MAX_POINTS) {
        printf("Invalid number of points.\n");
        return 0;
    }
    
    array->points = malloc((size_t)num_points * sizeof(struct Point));
    if (array->points == NULL) {
        printf("Memory allocation failed.\n");
        return 0;
    }
    
    array->count = (size_t)num_points;
    
    for (size_t i = 0; i < array->count; i++) {
        printf("Enter point %zu (x y): ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            free(array->points);
            return 0;
        }
        
        char *token = strtok(buffer, " \t\n");
        if (token == NULL) {
            printf("Invalid input format.\n");
            free(array->points);
            return 0;
        }
        
        array->points[i].x = strtod(token, &endptr);
        if (endptr == token || *endptr != '\0') {
            printf("Invalid x coordinate.\n");
            free(array->points);
            return 0;
        }
        
        token = strtok(NULL, " \t\n");
        if (token == NULL) {
            printf("Invalid input format.\n");
            free(array->points);
            return 0;
        }
        
        array->points[i].y = strtod(token, &endptr);
        if (endptr == token || *endptr != '\0') {
            printf("Invalid y coordinate.\n");
            free(array->points);
            return 0;
        }
    }
    
    return 1;
}

void analyze_point_distances(struct PointArray *array) {
    if (array->count < 2) {
        printf("Need at least 2 points for distance analysis.\n");
        return;
    }
    
    double min_distance = -1.0;
    double max_distance = 0.0;
    double total_distance = 0.0;
    size_t distance_count = 0;
    
    for (size_t i = 0; i < array->count; i++) {
        for (size_t j = i + 1; j < array->count; j++) {
            double dist = calculate_distance(array->points[i], array->points[j]);
            
            if (min_distance < 0 || dist < min_distance) {
                min_distance = dist;
            }
            if (dist > max_distance) {
                max_distance = dist;
            }
            
            total_distance += dist;
            distance_count++;
        }
    }
    
    printf("Distance Analysis Results:\n");
    printf("Minimum distance: %.6f\n", min_distance);
    printf("Maximum distance: %.6f\n", max_distance);
    printf("Average distance: %.6f\n", total_distance / distance_count);
}

int main(void) {
    struct PointArray point_array;
    point_array.points = NULL;
    point_array.count = 0;
    
    struct Point stack_points[10];
    size_t stack_count = 0;
    
    printf("Point Distance Analyzer\n");
    printf("=======================\n\n");
    
    if (!read_points_from_user(&point_array)) {
        return 1;
    }
    
    analyze_point_distances(&point_array);
    
    if (point_array.count > 0) {
        for (size_t i = 0; i < point_array.count && i < 10; i++) {
            stack_points[i] = point_array.points[i];
            stack_count++;
        }
        
        printf("\nFirst %zu points (stored on stack):\n", stack_count);
        for (size_t i = 0; i < stack_count; i++) {
            printf("Point %zu: (%.2f, %.2f)\n", i + 1, stack_points[i].x, stack_points[i].y);
        }
    }
    
    if (point_array.points != NULL) {
        free(point_array.points);
    }
    
    return 0;
}