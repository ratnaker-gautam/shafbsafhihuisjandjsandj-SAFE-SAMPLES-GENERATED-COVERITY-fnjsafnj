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

int main() {
    struct Point points_stack[MAX_POINTS];
    int point_count = 0;
    char buffer[100];
    
    printf("Enter points (x y), one per line. Enter 'done' to finish:\n");
    
    while (point_count < MAX_POINTS) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "done") == 0) break;
        
        char* space = strchr(buffer, ' ');
        if (space == NULL) {
            printf("Invalid format. Use 'x y'\n");
            continue;
        }
        
        *space = '\0';
        char* x_str = buffer;
        char* y_str = space + 1;
        
        if (!validate_input(x_str) || !validate_input(y_str)) {
            printf("Invalid number format\n");
            continue;
        }
        
        double x = parse_double(x_str);
        double y = parse_double(y_str);
        
        if (x < -1e6 || x > 1e6 || y < -1e6 || y > 1e6) {
            printf("Coordinates out of range\n");
            continue;
        }
        
        points_stack[point_count].x = x;
        points_stack[point_count].y = y;
        point_count++;
    }
    
    if (point_count == 0) {
        printf("No points entered\n");
        return 0;
    }
    
    int k = 3;
    if (point_count < k) k = point_count;
    
    struct Cluster* clusters = malloc(k * sizeof(struct Cluster));
    if (clusters == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < k; i++) {
        clusters[i].center_x = points_stack[i].x;
        clusters[i].center_y = points_stack[i].y;
        clusters[i].points = malloc(point_count * sizeof(struct Point));
        if (clusters[i].points == NULL) {
            printf("Memory allocation failed\n");
            for (int j = 0; j < i; j++) free(clusters[j].points);
            free(clusters);
            return 1;
        }
        clusters[i].count = 0;
        clusters[i].capacity = point_count;
    }
    
    int max_iterations = 100;
    int changed;
    
    for (int iter = 0; iter < max_iterations; iter++) {
        changed = 0;
        
        for (int i = 0; i < k; i++) {
            clusters[i].count = 0;
        }
        
        for (int i = 0; i < point_count; i++) {
            double min_dist = -1.0;
            int best_cluster = 0;
            
            for (int j = 0; j < k; j++) {
                double dist = calculate_distance(points_stack[i].x, points_stack[i].y,
                                               clusters[j].center_x, clusters[j].center_y);
                if (