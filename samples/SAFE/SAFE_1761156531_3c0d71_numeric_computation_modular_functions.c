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
    double distance_squared = dx * dx + dy * dy;
    
    if (distance_squared < 0) {
        return 0.0;
    }
    
    return sqrt(distance_squared);
}

double find_minimum_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double min_distance = -1.0;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double distance = calculate_distance(points[i], points[j]);
            
            if (min_distance < 0 || distance < min_distance) {
                min_distance = distance;
            }
        }
    }
    
    return min_distance;
}

double find_maximum_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double max_distance = 0.0;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double distance = calculate_distance(points[i], points[j]);
            
            if (distance > max_distance) {
                max_distance = distance;
            }
        }
    }
    
    return max_distance;
}

double calculate_average_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double total_distance = 0.0;
    int pair_count = 0;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            total_distance += calculate_distance(points[i], points[j]);
            pair_count++;
        }
    }
    
    if (pair_count == 0) {
        return 0.0;
    }
    
    return total_distance / pair_count;
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
        printf("Invalid number of points.\n");
        return 1;
    }
    
    printf("Enter coordinates for %d points (x y):\n", point_count);
    for (int i = 0; i < point_count; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            printf("Invalid coordinate format.\n");
            return 1;
        }
        
        if (!validate_coordinate(points[i].x) || !validate_coordinate(points[i].y)) {
            printf("Coordinate out of valid range.\n");
            return 1;
        }
    }
    
    if (point_count == 1) {
        printf("Only one point provided. Distance statistics require at least two points.\n");
        return 0;
    }
    
    double min_dist = find_minimum_distance(points, point_count);
    double max_dist = find_maximum_distance(points, point_count);
    double avg_dist = calculate_average_distance(points, point_count);
    
    printf("Distance Statistics:\n");
    printf("Minimum distance: %.6f\n", min_dist);
    printf("Maximum distance: %.6f\n", max_dist);
    printf("Average distance: %.6f\n", avg_dist);
    
    return 0;
}