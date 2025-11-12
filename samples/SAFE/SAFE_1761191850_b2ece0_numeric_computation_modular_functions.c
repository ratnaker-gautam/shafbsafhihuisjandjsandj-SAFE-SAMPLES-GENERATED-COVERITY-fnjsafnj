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

int read_points(struct Point points[], int count) {
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

double calculate_area(struct Point points[], int count) {
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

int main(void) {
    int point_count;
    
    printf("Enter number of points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &point_count) != 1) {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (!validate_input_count(point_count)) {
        printf("Invalid number of points\n");
        return 1;
    }
    
    struct Point points[MAX_POINTS];
    
    printf("Enter %d points as x y coordinates:\n", point_count);
    if (!read_points(points, point_count)) {
        printf("Error reading points\n");
        return 1;
    }
    
    double perimeter = calculate_total_distance(points, point_count);
    double area = calculate_area(points, point_count);
    
    printf("Polygon perimeter: %.6f\n", perimeter);
    printf("Polygon area: %.6f\n", area);
    
    return 0;
}