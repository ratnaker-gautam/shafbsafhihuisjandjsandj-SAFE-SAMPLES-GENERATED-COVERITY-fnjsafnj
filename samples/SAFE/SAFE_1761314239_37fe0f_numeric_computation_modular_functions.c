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

int read_points(struct Point* points, int count) {
    for (int i = 0; i < count; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            return 0;
        }
    }
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

double calculate_total_distance(struct Point* points, int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double total = 0.0;
    for (int i = 0; i < count - 1; i++) {
        total += calculate_distance(points[i], points[i + 1]);
    }
    return total;
}

double calculate_area(struct Point* points, int count) {
    if (count < 3) {
        return 0.0;
    }
    
    double area = 0.0;
    for (int i = 0; i < count; i++) {
        int j = (i + 1) % count;
        area += points[i].x * points[j].y - points[j].x * points[i].y;
    }
    return fabs(area) / 2.0;
}

int main() {
    int point_count;
    
    printf("Enter number of points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &point_count) != 1) {
        printf("Invalid input format.\n");
        return 1;
    }
    
    if (!validate_input_count(point_count)) {
        printf("Invalid number of points.\n");
        return 1;
    }
    
    struct Point points[MAX_POINTS];
    
    printf("Enter %d points as x y coordinates:\n", point_count);
    if (!read_points(points, point_count)) {
        printf("Error reading point coordinates.\n");
        return 1;
    }
    
    double total_distance = calculate_total_distance(points, point_count);
    printf("Total distance between consecutive points: %.6f\n", total_distance);
    
    if (point_count >= 3) {
        double polygon_area = calculate_area(points, point_count);
        printf("Area of polygon formed by points: %.6f\n", polygon_area);
    }
    
    double min_x = points[0].x;
    double max_x = points[0].x;
    double min_y = points[0].y;
    double max_y = points[0].y;
    
    for (int i = 1; i < point_count; i++) {
        if (points[i].x < min_x) min_x = points[i].x;
        if (points[i].x > max_x) max_x = points[i].x;
        if (points[i].y < min_y) min_y = points[i].y;
        if (points[i].y > max_y) max_y = points[i].y;
    }
    
    printf("Bounding box: (%.6f, %.6f) to (%.6f, %.6f)\n", min_x, min_y, max_x, max_y);
    
    return 0;
}