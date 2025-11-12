//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int graph[MAX_NODES][MAX_NODES];
int dist[MAX_NODES][MAX_NODES];

void initialize_graph(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                graph[i][j] = 0;
            } else {
                graph[i][j] = INF;
            }
        }
    }
}

void floyd_warshall(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i][j] = graph[i][j];
        }
    }
    
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

int main() {
    int n, m;
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    n = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    
    initialize_graph(n);
    
    m = read_int("Enter number of edges: ", 0, n * (n - 1));
    
    printf("\nEnter edges (from to weight):\n");
    for (int i = 0; i < m; i++) {
        int u, v, w;
        
        u = read_int("From node: ", 0, n - 1);
        v = read_int("To node: ", 0, n - 1);
        w = read_int("Weight: ", 1, 10000);
        
        if (u == v) {
            printf("Self-loops not allowed. Edge ignored.\n");
            i--;
            continue;
        }
        
        graph[u][v] = w;
    }
    
    floyd_warshall(n);
    
    printf("\nShortest path distances:\n");
    printf("    ");
    for (int i = 0; i < n; i++) {
        printf("%4d", i);
    }
    printf("\n");
    
    for (int i = 0; i < n; i++) {
        printf("%4d", i);
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF) {
                printf("   -");
            } else {
                printf("%4d", dist[i][j]);
            }
        }
        printf("\n");
    }
    
    printf("\nPath queries:\n");
    while (1) {
        int src, dest;
        char choice;
        
        src = read_int("Enter source node (-1 to exit): ", -1, n - 1);
        if (src == -1) break;
        
        dest = read_int("Enter destination node: ", 0, n - 1);
        
        if (dist[src][dest] == INF) {
            printf("No path exists from %d to %d\n", src, dest);
        } else {
            printf("Shortest distance from %d to %d: %d\n", src, dest, dist[src][dest]);
        }
        
        printf("Another query? (y/n): ");
        if (scanf(" %c", &choice) != 1) {
            while (getchar() != '\n');
            break;
        }
        while (getchar() != '\n');
        
        if (choice != 'y' && choice != 'Y') {
            break;
        }
    }
    
    return 0;
}