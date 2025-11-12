//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_POINTS 1000
#define MAX_DIMENSIONS 10

struct Point {
    double *coordinates;
    int dimensions;
};

struct Cluster {
    struct Point centroid;
    int point_count;
};

double calculate_distance(const struct Point *p1, const struct Point *p2) {
    if (p1 == NULL || p2 == NULL || p1->dimensions != p2->dimensions) {
        return -1.0;
    }
    
    double sum = 0.0;
    for (int i = 0; i < p1->dimensions; i++) {
        double diff = p1->coordinates[i] - p2->coordinates[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

int assign_to_cluster(const struct Point *point, struct Cluster *clusters, int k) {
    if (point == NULL || clusters == NULL || k <= 0) {
        return -1;
    }
    
    int closest_cluster = 0;
    double min_distance = calculate_distance(point, &clusters[0].centroid);
    
    for (int i = 1; i < k; i++) {
        double distance = calculate_distance(point, &clusters[i].centroid);
        if (distance >= 0.0 && distance < min_distance) {
            min_distance = distance;
            closest_cluster = i;
        }
    }
    
    return closest_cluster;
}

void update_centroids(struct Point *points, int n, struct Cluster *clusters, int k) {
    if (points == NULL || clusters == NULL || n <= 0 || k <= 0) {
        return;
    }
    
    for (int i = 0; i < k; i++) {
        clusters[i].point_count = 0;
        for (int j = 0; j < clusters[i].centroid.dimensions; j++) {
            clusters[i].centroid.coordinates[j] = 0.0;
        }
    }
    
    for (int i = 0; i < n; i++) {
        int cluster_idx = assign_to_cluster(&points[i], clusters, k);
        if (cluster_idx >= 0 && cluster_idx < k) {
            clusters[cluster_idx].point_count++;
            for (int j = 0; j < points[i].dimensions; j++) {
                clusters[cluster_idx].centroid.coordinates[j] += points[i].coordinates[j];
            }
        }
    }
    
    for (int i = 0; i < k; i++) {
        if (clusters[i].point_count > 0) {
            for (int j = 0; j < clusters[i].centroid.dimensions; j++) {
                clusters[i].centroid.coordinates[j] /= clusters[i].point_count;
            }
        }
    }
}

int main(void) {
    int n, dimensions, k;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_POINTS) {
        printf("Invalid number of points\n");
        return 1;
    }
    
    printf("Enter number of dimensions (1-%d): ", MAX_DIMENSIONS);
    if (scanf("%d", &dimensions) != 1 || dimensions < 1 || dimensions > MAX_DIMENSIONS) {
        printf("Invalid number of dimensions\n");
        return 1;
    }
    
    printf("Enter number of clusters (1-%d): ", n);
    if (scanf("%d", &k) != 1 || k < 1 || k > n) {
        printf("Invalid number of clusters\n");
        return 1;
    }
    
    struct Point *points = malloc(n * sizeof(struct Point));
    if (points == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < n; i++) {
        points[i].coordinates = malloc(dimensions * sizeof(double));
        if (points[i].coordinates == NULL) {
            printf("Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(points[j].coordinates);
            }
            free(points);
            return 1;
        }
        points[i].dimensions = dimensions;
        
        printf("Enter coordinates for point %d: ", i + 1);
        for (int j = 0; j < dimensions; j++) {
            if (scanf("%lf", &points[i].coordinates[j]) != 1) {
                printf("Invalid coordinate\n");
                for (int x = 0; x <= i; x++) {
                    free(points[x].coordinates);
                }
                free(points);
                return 1;
            }
        }
    }
    
    struct Cluster *clusters = malloc(k * sizeof(struct Cluster));
    if (clusters == NULL) {
        printf("Memory allocation failed\n");
        for (int i = 0; i < n; i++) {
            free(points[i].coordinates);
        }
        free(points);
        return 1;
    }
    
    for (int i = 0; i < k; i++) {
        clusters[i].centroid.coordinates = malloc(dimensions * sizeof(double));
        if (clusters[i].centroid.coordinates == NULL) {
            printf("Memory allocation failed\n");
            for (int j = 0; j < i; j