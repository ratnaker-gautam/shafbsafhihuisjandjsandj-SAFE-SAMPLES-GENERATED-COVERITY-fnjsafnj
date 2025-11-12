//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_POINTS 1000

struct Point {
    double x;
    double y;
};

struct Cluster {
    double centroid_x;
    double centroid_y;
    struct Point* points;
    int count;
    int capacity;
};

int read_points_from_input(struct Point* points, int max_points) {
    int count = 0;
    char buffer[256];
    
    printf("Enter points as 'x y' (one per line, empty line to finish):\n");
    
    while (count < max_points && fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n' || buffer[0] == '\0') {
            break;
        }
        
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            points[count].x = x;
            points[count].y = y;
            count++;
        } else {
            printf("Invalid input. Please enter 'x y' format.\n");
        }
    }
    
    return count;
}

void initialize_clusters(struct Cluster* clusters, int k, struct Point* initial_points) {
    for (int i = 0; i < k; i++) {
        clusters[i].centroid_x = initial_points[i].x;
        clusters[i].centroid_y = initial_points[i].y;
        clusters[i].points = malloc(sizeof(struct Point) * 10);
        clusters[i].count = 0;
        clusters[i].capacity = 10;
        
        if (clusters[i].points == NULL) {
            for (int j = 0; j < i; j++) {
                free(clusters[j].points);
            }
            exit(EXIT_FAILURE);
        }
    }
}

double calculate_distance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

void assign_points_to_clusters(struct Point* points, int point_count, 
                              struct Cluster* clusters, int k) {
    for (int i = 0; i < k; i++) {
        clusters[i].count = 0;
    }
    
    for (int i = 0; i < point_count; i++) {
        double min_distance = -1.0;
        int closest_cluster = 0;
        
        for (int j = 0; j < k; j++) {
            double dist = calculate_distance(points[i].x, points[i].y,
                                           clusters[j].centroid_x, clusters[j].centroid_y);
            if (min_distance < 0 || dist < min_distance) {
                min_distance = dist;
                closest_cluster = j;
            }
        }
        
        if (clusters[closest_cluster].count >= clusters[closest_cluster].capacity) {
            int new_capacity = clusters[closest_cluster].capacity * 2;
            struct Point* new_points = realloc(clusters[closest_cluster].points,
                                             sizeof(struct Point) * new_capacity);
            if (new_points == NULL) {
                exit(EXIT_FAILURE);
            }
            clusters[closest_cluster].points = new_points;
            clusters[closest_cluster].capacity = new_capacity;
        }
        
        clusters[closest_cluster].points[clusters[closest_cluster].count] = points[i];
        clusters[closest_cluster].count++;
    }
}

int update_centroids(struct Cluster* clusters, int k) {
    int changed = 0;
    
    for (int i = 0; i < k; i++) {
        if (clusters[i].count == 0) {
            continue;
        }
        
        double sum_x = 0.0;
        double sum_y = 0.0;
        
        for (int j = 0; j < clusters[i].count; j++) {
            sum_x += clusters[i].points[j].x;
            sum_y += clusters[i].points[j].y;
        }
        
        double new_centroid_x = sum_x / clusters[i].count;
        double new_centroid_y = sum_y / clusters[i].count;
        
        if (fabs(new_centroid_x - clusters[i].centroid_x) > 0.001 ||
            fabs(new_centroid_y - clusters[i].centroid_y) > 0.001) {
            changed = 1;
        }
        
        clusters[i].centroid_x = new_centroid_x;
        clusters[i].centroid_y = new_centroid_y;
    }
    
    return changed;
}

void print_clusters(struct Cluster* clusters, int k) {
    printf("\nFinal clusters:\n");
    for (int i = 0; i < k; i++) {
        printf("Cluster %d: centroid (%.2f, %.2f), %d points\n",
               i + 1, clusters[i].centroid_x, clusters[i].centroid_y, clusters[i].count);
        
        for (int j = 0; j < clusters[i].count; j++) {
            printf("  (%.2f, %.2f)\n", clusters[i].points[j].x, clusters[i].points[j].y);
        }
    }
}

void cleanup_clusters(struct Cluster* clusters, int k) {
    for (int i = 0; i < k; i++) {