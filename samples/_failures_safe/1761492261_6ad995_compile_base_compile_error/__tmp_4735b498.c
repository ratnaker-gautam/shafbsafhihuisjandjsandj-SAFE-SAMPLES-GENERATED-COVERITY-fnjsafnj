//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_POINTS 1000
#define MAX_ITERATIONS 100

struct Point {
    double x;
    double y;
};

struct PointArray {
    struct Point *points;
    size_t count;
};

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_user(struct PointArray *array) {
    char buffer[256];
    size_t count;
    
    printf("Enter number of points (1-%zu): ", (size_t)MAX_POINTS);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    if (sscanf(buffer, "%zu", &count) != 1) {
        return 0;
    }
    
    if (count < 1 || count > MAX_POINTS) {
        return 0;
    }
    
    array->points = malloc(count * sizeof(struct Point));
    if (array->points == NULL) {
        return 0;
    }
    
    array->count = count;
    
    for (size_t i = 0; i < count; i++) {
        printf("Enter point %zu (x y): ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            free(array->points);
            return 0;
        }
        
        if (sscanf(buffer, "%lf %lf", &array->points[i].x, &array->points[i].y) != 2) {
            free(array->points);
            return 0;
        }
    }
    
    return 1;
}

void compute_centroid(struct PointArray *points, struct Point *centroid) {
    double sum_x = 0.0;
    double sum_y = 0.0;
    
    for (size_t i = 0; i < points->count; i++) {
        sum_x += points->points[i].x;
        sum_y += points->points[i].y;
    }
    
    centroid->x = sum_x / points->count;
    centroid->y = sum_y / points->count;
}

void kmeans_iteration(struct PointArray *points, struct Point *centroids, size_t k, int *assignments) {
    for (size_t i = 0; i < points->count; i++) {
        double min_dist = -1.0;
        int best_cluster = -1;
        
        for (size_t j = 0; j < k; j++) {
            double dist = calculate_distance(points->points[i], centroids[j]);
            if (best_cluster == -1 || dist < min_dist) {
                min_dist = dist;
                best_cluster = j;
            }
        }
        assignments[i] = best_cluster;
    }
    
    size_t *counts = calloc(k, sizeof(size_t));
    if (counts == NULL) return;
    
    for (size_t j = 0; j < k; j++) {
        centroids[j].x = 0.0;
        centroids[j].y = 0.0;
    }
    
    for (size_t i = 0; i < points->count; i++) {
        int cluster = assignments[i];
        centroids[cluster].x += points->points[i].x;
        centroids[cluster].y += points->points[i].y;
        counts[cluster]++;
    }
    
    for (size_t j = 0; j < k; j++) {
        if (counts[j] > 0) {
            centroids[j].x /= counts[j];
            centroids[j].y /= counts[j];
        }
    }
    
    free(counts);
}

int main(void) {
    struct PointArray points;
    struct Point initial_centroids[3];
    int assignments[MAX_POINTS];
    size_t k = 3;
    
    if (!read_points_from_user(&points)) {
        printf("Error reading points.\n");
        return 1;
    }
    
    if (points.count < k) {
        printf("Need at least %zu points for clustering.\n", k);
        free(points.points);
        return 1;
    }
    
    for (size_t i = 0; i < k; i++) {
        initial_centroids[i] = points.points[i % points.count];
    }
    
    struct Point *centroids = malloc(k * sizeof(struct Point));
    if (centroids == NULL) {
        free(points.points);
        return 1;
    }
    
    memcpy(centroids, initial_centroids, k * sizeof(struct Point));
    
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        kmeans_iteration(&points, centroids, k, assignments);
    }
    
    printf("\nFinal centroids:\n");
    for (size_t i = 0; i < k; i++) {
        printf("Cluster %zu: (%.2f, %.2f)\n", i + 1, centroids[i].x, centroids[i].y);
    }
    
    printf("\nPoint assignments:\n");
    for (size_t i = 0; i < points.count; i++) {
        printf("Point %zu (%.2f, %.2f) -> Cluster %d\n", 
               i + 1, points.points