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

int validate_point_coordinates(double x, double y) {
    if (x < -1e6 || x > 1e6 || y < -1e6 || y > 1e6) {
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
    for (int i = 0; i < count - 1; i++) {
        total += calculate_distance(points[i], points[i + 1]);
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
    
    printf("Enter coordinates for %d points (x y):\n", count);
    for (int i = 0; i < count; i++) {
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            return -1;
        }
        
        if (!validate_point_coordinates(x, y)) {
            return -1;
        }
        
        points[i].x = x;
        points[i].y = y;
    }
    
    return count;
}

void print_points(struct Point points[], int count) {
    printf("Points entered:\n");
    for (int i = 0; i < count; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i + 1, points[i].x, points[i].y);
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int count = read_points(points, MAX_POINTS);
    if (count < 0) {
        printf("Error: Invalid input. Please enter valid coordinates.\n");
        return 1;
    }
    
    print_points(points, count);
    
    double total_distance = calculate_total_distance(points, count);
    printf("Total distance along the path: %.4f\n", total_distance);
    
    if (count >= 3) {
        double perimeter = total_distance + calculate_distance(points[count - 1], points[0]);
        printf("Perimeter of the polygon: %.4f\n", perimeter);
    }
    
    return 0;
}