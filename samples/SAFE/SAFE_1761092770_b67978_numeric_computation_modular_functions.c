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
    
    if (dx > 1e150 || dy > 1e150 || dx < -1e150 || dy < -1e150) {
        return -1.0;
    }
    
    double sum_squares = dx * dx + dy * dy;
    if (sum_squares > 1e300) {
        return -1.0;
    }
    
    return sqrt(sum_squares);
}

double calculate_total_path_length(struct Point* points, int count) {
    if (points == NULL || count < 2) {
        return 0.0;
    }
    
    double total_length = 0.0;
    for (int i = 0; i < count - 1; i++) {
        double distance = calculate_distance(points[i], points[i+1]);
        if (distance < 0) {
            return -1.0;
        }
        total_length += distance;
    }
    
    return total_length;
}

int read_points(struct Point* points, int count) {
    if (points == NULL || count <= 0) {
        return 0;
    }
    
    printf("Enter %d points (x y coordinates):\n", count);
    for (int i = 0; i < count; i++) {
        printf("Point %d: ", i + 1);
        
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            return 0;
        }
        
        if (!validate_coordinate(points[i].x) || !validate_coordinate(points[i].y)) {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    int point_count;
    
    printf("Enter number of points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &point_count) != 1) {
        printf("Error: Invalid input format.\n");
        return 1;
    }
    
    if (!validate_input_count(point_count)) {
        printf("Error: Invalid number of points.\n");
        return 1;
    }
    
    struct Point points[MAX_POINTS];
    
    if (!read_points(points, point_count)) {
        printf("Error: Invalid point coordinates.\n");
        return 1;
    }
    
    double path_length = calculate_total_path_length(points, point_count);
    
    if (path_length < 0) {
        printf("Error: Numerical overflow in distance calculation.\n");
        return 1;
    }
    
    printf("Total path length: %.6f\n", path_length);
    
    return 0;
}