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
    if (x < -1e100 || x > 1e100 || y < -1e100 || y > 1e100) {
        return 0;
    }
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    
    if (dx > 1e100 || dx < -1e100 || dy > 1e100 || dy < -1e100) {
        return -1.0;
    }
    
    double squared_sum = dx * dx + dy * dy;
    if (squared_sum > 1e200 || squared_sum < 0) {
        return -1.0;
    }
    
    return sqrt(squared_sum);
}

double calculate_total_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double total = 0.0;
    for (int i = 0; i < count - 1; i++) {
        double distance = calculate_distance(points[i], points[i+1]);
        if (distance < 0) {
            return -1.0;
        }
        total += distance;
    }
    return total;
}

int read_points(struct Point points[], int max_count) {
    int count;
    printf("Enter number of points (2-%d): ", max_count);
    if (scanf("%d", &count) != 1) {
        return -1;
    }
    
    if (!validate_input_count(count)) {
        return -1;
    }
    
    printf("Enter %d points as x y coordinates:\n", count);
    for (int i = 0; i < count; i++) {
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            return -1;
        }
        
        if (!validate_point_input(x, y)) {
            return -1;
        }
        
        points[i].x = x;
        points[i].y = y;
    }
    
    return count;
}

void print_results(struct Point points[], int count, double total_distance) {
    printf("\nPoints entered:\n");
    for (int i = 0; i < count; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i+1, points[i].x, points[i].y);
    }
    
    printf("\nDistances between consecutive points:\n");
    for (int i = 0; i < count - 1; i++) {
        double dist = calculate_distance(points[i], points[i+1]);
        printf("Distance from point %d to %d: %.2f\n", i+1, i+2, dist);
    }
    
    printf("\nTotal distance along the path: %.2f\n", total_distance);
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int point_count = read_points(points, MAX_POINTS);
    if (point_count < 0) {
        printf("Error: Invalid input. Please enter valid numeric values within reasonable bounds.\n");
        return 1;
    }
    
    double total_distance = calculate_total_distance(points, point_count);
    if (total_distance < 0) {
        printf("Error: Distance calculation overflow or invalid values.\n");
        return 1;
    }
    
    print_results(points, point_count, total_distance);
    
    return 0;
}