//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_POINTS 1000
#define MAX_DIMENSIONS 10

struct Point {
    double coordinates[MAX_DIMENSIONS];
    int dimension;
};

struct Cluster {
    struct Point centroid;
    int point_count;
    double sum[MAX_DIMENSIONS];
};

double calculate_distance(const struct Point* p1, const struct Point* p2) {
    if (p1 == NULL || p2 == NULL || p1->dimension != p2->dimension) {
        return -1.0;
    }
    
    double distance_squared = 0.0;
    for (int i = 0; i < p1->dimension; i++) {
        double diff = p1->coordinates[i] - p2->coordinates[i];
        distance_squared += diff * diff;
    }
    
    return sqrt(distance_squared);
}

int find_nearest_cluster(const struct Point* point, struct Cluster* clusters, int cluster_count) {
    if (point == NULL || clusters == NULL || cluster_count <= 0) {
        return -1;
    }
    
    int nearest_index = 0;
    double min_distance = calculate_distance(point, &clusters[0].centroid);
    
    for (int i = 1; i < cluster_count; i++) {
        double current_distance = calculate_distance(point, &clusters[i].centroid);
        if (current_distance >= 0.0 && current_distance < min_distance) {
            min_distance = current_distance;
            nearest_index = i;
        }
    }
    
    return nearest_index;
}

void update_centroids(struct Cluster* clusters, int cluster_count) {
    if (clusters == NULL || cluster_count <= 0) {
        return;
    }
    
    for (int i = 0; i < cluster_count; i++) {
        if (clusters[i].point_count > 0) {
            for (int j = 0; j < clusters[i].centroid.dimension; j++) {
                clusters[i].centroid.coordinates[j] = clusters[i].sum[j] / clusters[i].point_count;
            }
        }
    }
}

int main() {
    int point_count, dimension, cluster_count;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &point_count) != 1 || point_count < 1 || point_count > MAX_POINTS) {
        printf("Invalid number of points\n");
        return 1;
    }
    
    printf("Enter dimension (1-%d): ", MAX_DIMENSIONS);
    if (scanf("%d", &dimension) != 1 || dimension < 1 || dimension > MAX_DIMENSIONS) {
        printf("Invalid dimension\n");
        return 1;
    }
    
    printf("Enter number of clusters (1-%d): ", point_count);
    if (scanf("%d", &cluster_count) != 1 || cluster_count < 1 || cluster_count > point_count) {
        printf("Invalid number of clusters\n");
        return 1;
    }
    
    struct Point* points = malloc(point_count * sizeof(struct Point));
    if (points == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter %d points with %d coordinates each:\n", point_count, dimension);
    for (int i = 0; i < point_count; i++) {
        points[i].dimension = dimension;
        for (int j = 0; j < dimension; j++) {
            if (scanf("%lf", &points[i].coordinates[j]) != 1) {
                printf("Invalid coordinate\n");
                free(points);
                return 1;
            }
        }
    }
    
    struct Cluster* clusters = malloc(cluster_count * sizeof(struct Cluster));
    if (clusters == NULL) {
        printf("Memory allocation failed\n");
        free(points);
        return 1;
    }
    
    for (int i = 0; i < cluster_count; i++) {
        clusters[i].centroid.dimension = dimension;
        clusters[i].point_count = 0;
        for (int j = 0; j < dimension; j++) {
            clusters[i].sum[j] = 0.0;
            clusters[i].centroid.coordinates[j] = points[i % point_count].coordinates[j];
        }
    }
    
    int max_iterations = 100;
    int changed;
    
    for (int iter = 0; iter < max_iterations; iter++) {
        changed = 0;
        
        for (int i = 0; i < cluster_count; i++) {
            clusters[i].point_count = 0;
            for (int j = 0; j < dimension; j++) {
                clusters[i].sum[j] = 0.0;
            }
        }
        
        for (int i = 0; i < point_count; i++) {
            int nearest = find_nearest_cluster(&points[i], clusters, cluster_count);
            if (nearest >= 0) {
                clusters[nearest].point_count++;
                for (int j = 0; j < dimension; j++) {
                    clusters[nearest].sum[j] += points[i].coordinates[j];
                }
                changed = 1;
            }
        }
        
        if (!changed) {
            break;
        }
        
        update_centroids(clusters, cluster_count);
    }
    
    printf("\nFinal cluster centroids:\n");
    for (int i = 0; i