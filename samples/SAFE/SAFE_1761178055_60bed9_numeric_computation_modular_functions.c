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

double calculate_total_path_length(struct Point points[], int count) {
    if (count < 2) {
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

void print_results(struct Point points[], int count, double total_length) {
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
    
    int count = read_points(points, MAX_POINTS);
    if (count < 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    double total_length = calculate_total_path_length(points, count);
    if (total_length < 0) {
        printf("Error: Calculation overflow\n");
        return 1;
    }
    
    print_results(points, count, total_length);
    
    return 0;
}