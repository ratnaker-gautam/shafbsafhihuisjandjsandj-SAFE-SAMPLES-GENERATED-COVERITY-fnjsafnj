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

int validate_coordinate(double coord) {
    if (coord < -1e6 || coord > 1e6) {
        return 0;
    }
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

double calculate_total_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double total = 0.0;
    for (int i = 1; i < count; i++) {
        total += calculate_distance(points[i-1], points[i]);
    }
    return total;
}

double calculate_area_triangle(struct Point a, struct Point b, struct Point c) {
    double area = fabs((a.x*(b.y-c.y) + b.x*(c.y-a.y) + c.x*(a.y-b.y)) / 2.0);
    return area;
}

double calculate_polygon_area(struct Point points[], int count) {
    if (count < 3) {
        return 0.0;
    }
    
    double area = 0.0;
    for (int i = 1; i < count - 1; i++) {
        area += calculate_area_triangle(points[0], points[i], points[i+1]);
    }
    return area;
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
    
    printf("Enter coordinates for %d points (x y):\n", count);
    for (int i = 0; i < count; i++) {
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            return -1;
        }
        
        if (!validate_coordinate(x) || !validate_coordinate(y)) {
            return -1;
        }
        
        points[i].x = x;
        points[i].y = y;
    }
    
    return count;
}

void print_results(struct Point points[], int count) {
    if (count < 1) {
        return;
    }
    
    printf("\nPoint coordinates:\n");
    for (int i = 0; i < count; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i+1, points[i].x, points[i].y);
    }
    
    if (count >= 2) {
        double total_dist = calculate_total_distance(points, count);
        printf("Total distance between consecutive points: %.4f\n", total_dist);
    }
    
    if (count >= 3) {
        double area = calculate_polygon_area(points, count);
        printf("Polygon area: %.4f\n", area);
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    printf("Polygon Distance and Area Calculator\n");
    printf("====================================\n");
    
    int count = read_points(points, MAX_POINTS);
    
    if (count <= 0) {
        printf("Error: Invalid input. Please enter valid coordinates.\n");
        return 1;
    }
    
    print_results(points, count);
    
    return 0;
}