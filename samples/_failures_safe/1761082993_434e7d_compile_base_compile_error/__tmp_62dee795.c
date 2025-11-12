//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
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

struct PointCluster {
    struct Point *points;
    int count;
    int capacity;
};

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIMENSIONS;
}

int validate_point_count(int count) {
    return count > 0 && count <= MAX_POINTS;
}

double calculate_distance(const struct Point *a, const struct Point *b) {
    if (a->dimension != b->dimension) return -1.0;
    
    double sum = 0.0;
    for (int i = 0; i < a->dimension; i++) {
        double diff = a->coordinates[i] - b->coordinates[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

int initialize_cluster(struct PointCluster *cluster, int initial_capacity) {
    if (initial_capacity <= 0 || initial_capacity > MAX_POINTS) return 0;
    
    cluster->points = malloc(initial_capacity * sizeof(struct Point));
    if (!cluster->points) return 0;
    
    cluster->count = 0;
    cluster->capacity = initial_capacity;
    return 1;
}

void add_point_to_cluster(struct PointCluster *cluster, const struct Point *point) {
    if (cluster->count >= cluster->capacity) {
        int new_capacity = cluster->capacity * 2;
        if (new_capacity > MAX_POINTS) new_capacity = MAX_POINTS;
        
        struct Point *new_points = realloc(cluster->points, new_capacity * sizeof(struct Point));
        if (!new_points) return;
        
        cluster->points = new_points;
        cluster->capacity = new_capacity;
    }
    
    if (cluster->count < cluster->capacity) {
        cluster->points[cluster->count] = *point;
        cluster->count++;
    }
}

void compute_cluster_statistics(const struct PointCluster *cluster, double *centroid, double *max_distance) {
    if (cluster->count == 0) {
        *max_distance = 0.0;
        return;
    }
    
    for (int i = 0; i < cluster->points[0].dimension; i++) {
        centroid[i] = 0.0;
    }
    
    for (int i = 0; i < cluster->count; i++) {
        for (int j = 0; j < cluster->points[i].dimension; j++) {
            centroid[j] += cluster->points[i].coordinates[j];
        }
    }
    
    for (int i = 0; i < cluster->points[0].dimension; i++) {
        centroid[i] /= cluster->count;
    }
    
    *max_distance = 0.0;
    struct Point center;
    center.dimension = cluster->points[0].dimension;
    for (int i = 0; i < center.dimension; i++) {
        center.coordinates[i] = centroid[i];
    }
    
    for (int i = 0; i < cluster->count; i++) {
        double dist = calculate_distance(&cluster->points[i], &center);
        if (dist > *max_distance) {
            *max_distance = dist;
        }
    }
}

void cleanup_cluster(struct PointCluster *cluster) {
    free(cluster->points);
    cluster->points = NULL;
    cluster->count = 0;
    cluster->capacity = 0;
}

int main() {
    int dimension, point_count;
    
    printf("Enter point dimension (1-%d): ", MAX_DIMENSIONS);
    if (scanf("%d", &dimension) != 1 || !validate_dimension(dimension)) {
        printf("Invalid dimension\n");
        return 1;
    }
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &point_count) != 1 || !validate_point_count(point_count)) {
        printf("Invalid point count\n");
        return 1;
    }
    
    struct PointCluster cluster;
    if (!initialize_cluster(&cluster, point_count)) {
        printf("Failed to initialize cluster\n");
        return 1;
    }
    
    printf("Enter %d points with %d coordinates each:\n", point_count, dimension);
    for (int i = 0; i < point_count; i++) {
        struct Point point;
        point.dimension = dimension;
        
        printf("Point %d: ", i + 1);
        for (int j = 0; j < dimension; j++) {
            if (scanf("%lf", &point.coordinates[j]) != 1) {
                printf("Invalid coordinate\n");
                cleanup_cluster(&cluster);
                return 1;
            }
        }
        add_point_to_cluster(&cluster, &point);
    }
    
    double centroid[MAX_DIMENSIONS];
    double max_distance;
    compute_cluster_statistics(&cluster, centroid, &max_distance);
    
    printf("\nCluster Statistics:\n");
    printf("Number of points: %d\n", cluster.count);
    printf("Centroid: (");
    for (int i = 0; i < dimension; i++) {
        printf("%.3f", centroid[i]);
        if (i < dimension - 1) printf(", ");
    }
    printf(")\n");
    printf("Maximum