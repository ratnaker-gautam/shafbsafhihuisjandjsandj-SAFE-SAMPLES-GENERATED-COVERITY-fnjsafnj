//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100

struct Point {
    double x;
    double y;
};

int validate_input_count(int count) {
    if (count <= 0 || count > MAX_POINTS) {
        return 0;
    }
    return 1;
}

int validate_point_input(double x, double y) {
    if (x != x || y != y) {
        return 0;
    }
    if (x > 1e100 || x < -1e100 || y > 1e100 || y < -1e100) {
        return 0;
    }
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    
    if (dx > 1e50 || dx < -1e50 || dy > 1e50 || dy < -1e50) {
        return -1.0;
    }
    
    double squared_sum = dx * dx + dy * dy;
    if (squared_sum > 1e100) {
        return -1.0;
    }
    
    return sqrt(squared_sum);
}

double calculate_total_path_length(struct Point* points, int count) {
    if (points == NULL || count < 2) {
        return 0.0;
    }
    
    double total_length = 0.0;
    for (int i = 0; i < count - 1; i++) {
        double distance = calculate_distance(points[i], points[i + 1]);
        if (distance < 0) {
            return -1.0;
        }
        total_length += distance;
    }
    
    return total_length;
}

int read_points_from_user(struct Point* points, int* count) {
    printf("Enter number of points (2-%d): ", MAX_POINTS);
    if (scanf("%d", count) != 1) {
        return 0;
    }
    
    if (!validate_input_count(*count)) {
        return 0;
    }
    
    printf("Enter %d points as x y coordinates:\n", *count);
    for (int i = 0; i < *count; i++) {
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            return 0;
        }
        
        if (!validate_point_input(x, y)) {
            return 0;
        }
        
        points[i].x = x;
        points[i].y = y;
    }
    
    return 1;
}

void print_results(struct Point* points, int count, double total_length) {
    printf("\nPoints entered:\n");
    for (int i = 0; i < count; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i + 1, points[i].x, points[i].y);
    }
    
    printf("\nDistances between consecutive points:\n");
    for (int i = 0; i < count - 1; i++) {
        double distance = calculate_distance(points[i], points[i + 1]);
        printf("Distance from point %d to %d: %.2f\n", i + 1, i + 2, distance);
    }
    
    printf("\nTotal path length: %.2f\n", total_length);
}

int main(void) {
    struct Point points[MAX_POINTS];
    int point_count = 0;
    
    printf("Path Length Calculator\n");
    printf("=====================\n\n");
    
    if (!read_points_from_user(points, &point_count)) {
        printf("Error: Invalid input. Please enter valid numeric values within reasonable bounds.\n");
        return 1;
    }
    
    double total_length = calculate_total_path_length(points, point_count);
    if (total_length < 0) {
        printf("Error: Calculation resulted in overflow or invalid values.\n");
        return 1;
    }
    
    print_results(points, point_count, total_length);
    
    return 0;
}