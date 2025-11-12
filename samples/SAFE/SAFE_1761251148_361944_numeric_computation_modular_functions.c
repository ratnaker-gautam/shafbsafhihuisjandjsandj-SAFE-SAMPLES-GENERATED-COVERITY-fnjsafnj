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
    return sqrt(dx * dx + dy * dy);
}

double find_minimum_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double min_dist = -1.0;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (dist >= 0.0 && (min_dist < 0.0 || dist < min_dist)) {
                min_dist = dist;
            }
        }
    }
    
    return min_dist;
}

double calculate_total_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double total = 0.0;
    int valid_segments = 0;
    
    for (int i = 0; i < count - 1; i++) {
        double dist = calculate_distance(points[i], points[i + 1]);
        if (dist >= 0.0) {
            total += dist;
            valid_segments++;
        }
    }
    
    if (valid_segments == 0) {
        return 0.0;
    }
    
    return total;
}

int main(void) {
    struct Point points[MAX_POINTS];
    int point_count;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &point_count) != 1) {
        printf("Invalid input format.\n");
        return 1;
    }
    
    if (!validate_input_count(point_count)) {
        printf("Invalid point count.\n");
        return 1;
    }
    
    printf("Enter coordinates for %d points (x y):\n", point_count);
    for (int i = 0; i < point_count; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            printf("Invalid coordinate format.\n");
            return 1;
        }
        
        if (!validate_coordinate(points[i].x) || !validate_coordinate(points[i].y)) {
            printf("Invalid coordinate values.\n");
            return 1;
        }
    }
    
    double min_dist = find_minimum_distance(points, point_count);
    double total_dist = calculate_total_distance(points, point_count);
    
    if (min_dist < 0.0) {
        printf("Could not calculate minimum distance.\n");
    } else {
        printf("Minimum distance between any two points: %.6f\n", min_dist);
    }
    
    printf("Total distance along point sequence: %.6f\n", total_dist);
    
    return 0;
}