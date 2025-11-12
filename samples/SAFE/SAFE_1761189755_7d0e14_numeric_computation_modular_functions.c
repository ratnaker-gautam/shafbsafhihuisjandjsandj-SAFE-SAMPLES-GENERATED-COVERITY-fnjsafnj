//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100

struct Point {
    double x;
    double y;
};

int read_points(struct Point points[], int max_count) {
    int count;
    printf("Enter number of points (1-%d): ", max_count);
    if (scanf("%d", &count) != 1) {
        return -1;
    }
    if (count < 1 || count > max_count) {
        return -1;
    }
    
    for (int i = 0; i < count; i++) {
        printf("Enter point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            return -1;
        }
    }
    return count;
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

double calculate_area_triangle(struct Point p1, struct Point p2, struct Point p3) {
    double area = fabs((p1.x * (p2.y - p3.y) + 
                       p2.x * (p3.y - p1.y) + 
                       p3.x * (p1.y - p2.y)) / 2.0);
    return area;
}

double calculate_polygon_area(struct Point points[], int count) {
    if (count < 3) {
        return 0.0;
    }
    
    double area = 0.0;
    for (int i = 0; i < count; i++) {
        int j = (i + 1) % count;
        area += points[i].x * points[j].y;
        area -= points[j].x * points[i].y;
    }
    return fabs(area) / 2.0;
}

void print_results(struct Point points[], int count) {
    if (count < 1) {
        printf("No valid points to process.\n");
        return;
    }
    
    printf("\nPoint coordinates:\n");
    for (int i = 0; i < count; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i + 1, points[i].x, points[i].y);
    }
    
    if (count >= 2) {
        double total_dist = calculate_total_distance(points, count);
        printf("Total distance between consecutive points: %.4f\n", total_dist);
    }
    
    if (count >= 3) {
        double area = calculate_polygon_area(points, count);
        printf("Polygon area: %.4f\n", area);
    }
    
    if (count >= 3) {
        printf("Triangle areas:\n");
        for (int i = 0; i < count - 2; i++) {
            double tri_area = calculate_area_triangle(points[i], points[i + 1], points[i + 2]);
            printf("Triangle %d-%d-%d: %.4f\n", i + 1, i + 2, i + 3, tri_area);
        }
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    printf("2D Point Geometry Calculator\n");
    printf("============================\n");
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Error: Invalid input or too many points.\n");
        return 1;
    }
    
    print_results(points, count);
    
    return 0;
}