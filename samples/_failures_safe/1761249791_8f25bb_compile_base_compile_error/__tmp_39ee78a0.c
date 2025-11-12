//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_POINTS 1000
#define MAX_DIMENSIONS 10

struct Point {
    double *coordinates;
    int dimension;
};

struct Cluster {
    struct Point centroid;
    int point_count;
    double *sums;
};

double calculate_distance(const struct Point *p1, const struct Point *p2) {
    if (p1->dimension != p2->dimension) return -1.0;
    double sum = 0.0;
    for (int i = 0; i < p1->dimension; i++) {
        double diff = p1->coordinates[i] - p2->coordinates[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

int assign_to_cluster(const struct Point *point, struct Cluster *clusters, int k) {
    int best_cluster = 0;
    double min_distance = calculate_distance(point, &clusters[0].centroid);
    for (int i = 1; i < k; i++) {
        double dist = calculate_distance(point, &clusters[i].centroid);
        if (dist < min_distance) {
            min_distance = dist;
            best_cluster = i;
        }
    }
    return best_cluster;
}

void update_centroids(struct Cluster *clusters, int k) {
    for (int i = 0; i < k; i++) {
        if (clusters[i].point_count > 0) {
            for (int j = 0; j < clusters[i].centroid.dimension; j++) {
                clusters[i].centroid.coordinates[j] = clusters[i].sums[j] / clusters[i].point_count;
            }
        }
    }
}

int main() {
    int n, k, dimensions;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_POINTS) {
        printf("Invalid number of points\n");
        return 1;
    }
    
    printf("Enter number of clusters (1-%d): ", n);
    if (scanf("%d", &k) != 1 || k < 1 || k > n) {
        printf("Invalid number of clusters\n");
        return 1;
    }
    
    printf("Enter point dimensions (1-%d): ", MAX_DIMENSIONS);
    if (scanf("%d", &dimensions) != 1 || dimensions < 1 || dimensions > MAX_DIMENSIONS) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    struct Point *points = malloc(n * sizeof(struct Point));
    if (!points) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < n; i++) {
        points[i].coordinates = malloc(dimensions * sizeof(double));
        points[i].dimension = dimensions;
        if (!points[i].coordinates) {
            printf("Memory allocation failed\n");
            for (int j = 0; j < i; j++) free(points[j].coordinates);
            free(points);
            return 1;
        }
        
        printf("Enter %d coordinates for point %d: ", dimensions, i+1);
        for (int j = 0; j < dimensions; j++) {
            if (scanf("%lf", &points[i].coordinates[j]) != 1) {
                printf("Invalid coordinate\n");
                for (int x = 0; x <= i; x++) free(points[x].coordinates);
                free(points);
                return 1;
            }
        }
    }
    
    struct Cluster *clusters = malloc(k * sizeof(struct Cluster));
    if (!clusters) {
        printf("Memory allocation failed\n");
        for (int i = 0; i < n; i++) free(points[i].coordinates);
        free(points);
        return 1;
    }
    
    for (int i = 0; i < k; i++) {
        clusters[i].centroid.coordinates = malloc(dimensions * sizeof(double));
        clusters[i].centroid.dimension = dimensions;
        clusters[i].sums = malloc(dimensions * sizeof(double));
        clusters[i].point_count = 0;
        
        if (!clusters[i].centroid.coordinates || !clusters[i].sums) {
            printf("Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(clusters[j].centroid.coordinates);
                free(clusters[j].sums);
            }
            free(clusters);
            for (int j = 0; j < n; j++) free(points[j].coordinates);
            free(points);
            return 1;
        }
        
        for (int j = 0; j < dimensions; j++) {
            clusters[i].centroid.coordinates[j] = points[i % n].coordinates[j];
            clusters[i].sums[j] = 0.0;
        }
    }
    
    int *assignments = malloc(n * sizeof(int));
    if (!assignments) {
        printf("Memory allocation failed\n");
        for (int i = 0; i < k; i++) {
            free(clusters[i].centroid.coordinates);
            free(clusters[i].sums);
        }
        free(clusters);
        for (int i = 0; i < n; i++) free(points[i].coordinates);
        free(points);
        return 1;