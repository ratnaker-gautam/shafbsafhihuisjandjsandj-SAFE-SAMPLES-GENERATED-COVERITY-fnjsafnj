//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
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
    if (coord != coord) {
        return 0;
    }
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    
    if (dx > 1e150 || dy > 1e150) {
        return -1.0;
    }
    
    double sum_squares = dx * dx + dy * dy;
    
    if (sum_squares > 1e300) {
        return -1.0;
    }
    
    return sqrt(sum_squares);
}

double calculate_total_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double total = 0.0;
    for (int i = 0; i < count - 1; i++) {
        double distance = calculate_distance(points[i], points[i + 1]);
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

int main(void) {
    struct Point points[MAX_POINTS];
    
    int point_count = read_points(points, MAX_POINTS);
    if (point_count < 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    double total_distance = calculate_total_distance(points, point_count);
    if (total_distance < 0) {
        printf("Error: Distance calculation overflow\n");
        return 1;
    }
    
    printf("Total distance along the path: %.6f\n", total_distance);
    
    return 0;
}