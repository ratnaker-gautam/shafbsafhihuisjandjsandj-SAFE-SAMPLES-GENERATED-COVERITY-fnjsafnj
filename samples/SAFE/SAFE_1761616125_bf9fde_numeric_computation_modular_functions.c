//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_POINTS 100

struct Point {
    double x;
    double y;
};

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

double calculate_polygon_perimeter(struct Point points[], int num_points) {
    if (num_points < 3 || num_points > MAX_POINTS) {
        return -1.0;
    }
    
    double perimeter = 0.0;
    for (int i = 0; i < num_points; i++) {
        int next = (i + 1) % num_points;
        double dist = calculate_distance(points[i], points[next]);
        perimeter += dist;
    }
    
    return perimeter;
}

int read_points(struct Point points[], int max_points) {
    int num_points;
    
    printf("Enter number of points (3-%d): ", max_points);
    if (scanf("%d", &num_points) != 1) {
        return -1;
    }
    
    if (num_points < 3 || num_points > max_points) {
        return -1;
    }
    
    printf("Enter points as x y coordinates:\n");
    for (int i = 0; i < num_points; i++) {
        printf("Point %d: ", i + 1);
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            return -1;
        }
    }
    
    return num_points;
}

void print_points(struct Point points[], int num_points) {
    printf("Polygon points:\n");
    for (int i = 0; i < num_points; i++) {
        printf("  Point %d: (%.2f, %.2f)\n", i + 1, points[i].x, points[i].y);
    }
}

int main() {
    struct Point points[MAX_POINTS];
    
    printf("Polygon Perimeter Calculator\n");
    printf("============================\n\n");
    
    int num_points = read_points(points, MAX_POINTS);
    if (num_points == -1) {
        printf("Error: Invalid input or number of points out of range.\n");
        return 1;
    }
    
    print_points(points, num_points);
    
    double perimeter = calculate_polygon_perimeter(points, num_points);
    if (perimeter == -1.0) {
        printf("Error: Invalid polygon configuration.\n");
        return 1;
    }
    
    printf("\nPolygon perimeter: %.4f\n", perimeter);
    
    return 0;
}