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
    double center_x;
    double center_y;
    struct Point* points;
    int count;
    int capacity;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    int dot_count = 0;
    int digit_count = 0;
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (input[i] >= '0' && input[i] <= '9') {
            digit_count++;
        } else if (input[i] == '-' && i == 0) {
            continue;
        } else {
            return 0;
        }
    }
    return digit_count > 0;
}

double parse_double(const char* input) {
    double result = 0.0;
    double fraction = 0.0;
    int sign = 1;
    int i = 0;
    int decimal_places = 0;
    
    if (input[0] == '-') {
        sign = -1;
        i++;
    }
    
    while (input[i] != '\0' && input[i] != '.') {
        if (input[i] >= '0' && input[i] <= '9') {
            result = result * 10.0 + (input[i] - '0');
        }
        i++;
    }
    
    if (input[i] == '.') {
        i++;
        double divisor = 10.0;
        while (input[i] != '\0') {
            if (input[i] >= '0' && input[i] <= '9') {
                fraction += (input[i] - '0') / divisor;
                divisor *= 10.0;
                decimal_places++;
                if (decimal_places > 10) break;
            }
            i++;
        }
    }
    
    return sign * (result + fraction);
}

double calculate_distance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

void add_point_to_cluster(struct Cluster* cluster, double x, double y) {
    if (cluster->count >= cluster->capacity) {
        int new_capacity = cluster->capacity * 2;
        if (new_capacity > MAX_POINTS) new_capacity = MAX_POINTS;
        struct Point* new_points = malloc(new_capacity * sizeof(struct Point));
        if (new_points == NULL) return;
        
        for (int i = 0; i < cluster->count; i++) {
            new_points[i] = cluster->points[i];
        }
        free(cluster->points);
        cluster->points = new_points;
        cluster->capacity = new_capacity;
    }
    
    cluster->points[cluster->count].x = x;
    cluster->points[cluster->count].y = y;
    cluster->count++;
}

void kmeans_clustering(struct Point* points, int num_points, int k, int max_iterations) {
    if (num_points <= 0 || k <= 0 || k > num_points || max_iterations <= 0) {
        return;
    }
    
    struct Cluster* clusters = malloc(k * sizeof(struct Cluster));
    if (clusters == NULL) return;
    
    for (int i = 0; i < k; i++) {
        clusters[i].center_x = points[i].x;
        clusters[i].center_y = points[i].y;
        clusters[i].points = malloc(10 * sizeof(struct Point));
        clusters[i].count = 0;
        clusters[i].capacity = 10;
    }
    
    for (int iter = 0; iter < max_iterations; iter++) {
        for (int i = 0; i < k; i++) {
            clusters[i].count = 0;
        }
        
        for (int i = 0; i < num_points; i++) {
            int closest_cluster = 0;
            double min_distance = calculate_distance(points[i].x, points[i].y, 
                                                   clusters[0].center_x, clusters[0].center_y);
            
            for (int j = 1; j < k; j++) {
                double distance = calculate_distance(points[i].x, points[i].y,
                                                   clusters[j].center_x, clusters[j].center_y);
                if (distance < min_distance) {
                    min_distance = distance;
                    closest_cluster = j;
                }
            }
            
            add_point_to_cluster(&clusters[closest_cluster], points[i].x, points[i].y);
        }
        
        int centers_changed = 0;
        for (int i = 0; i < k; i++) {
            if (clusters[i].count == 0) continue;
            
            double sum_x = 0.0;
            double sum_y = 0.0;
            for (int j = 0; j < clusters[i].count; j++) {
                sum_x += clusters[i].points[j].x;
                sum_y += clusters[i].points[j].y;
            }
            
            double new_center_x = sum_x / clusters[i].count;
            double new_center_y = sum_y / clusters[i].count