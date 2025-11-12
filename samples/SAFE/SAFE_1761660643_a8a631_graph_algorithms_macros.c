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
                    dist[i][j] = MIN(dist[i][j], dist[i][k] + dist[k][j]);
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
            printf("Input error\n");
            exit(1);
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d\n", min, max);
    }
}

int main() {
    int n, m;
    
    n = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    initialize_graph(n);
    
    m = read_int("Enter number of edges: ", 0, n * (n - 1));
    
    printf("Enter edges (from to weight):\n");
    for (int i = 0; i < m; i++) {
        int u, v, w;
        u = read_int("From: ", 0, n - 1);
        v = read_int("To: ", 0, n - 1);
        w = read_int("Weight: ", -10000, 10000);
        
        if (u == v && w != 0) {
            printf("Self-loop must have weight 0\n");
            i--;
            continue;
        }
        
        graph[u][v] = w;
    }
    
    floyd_warshall(n);
    
    printf("\nShortest path distances:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%4d ", dist[i][j]);
            }
        }
        printf("\n");
    }
    
    printf("\nPath queries:\n");
    while (1) {
        int src, dest;
        printf("Enter source and destination (-1 -1 to exit): ");
        if (scanf("%d %d", &src, &dest) != 2) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (src == -1 && dest == -1) {
            break;
        }
        
        if (src < 0 || src >= n || dest < 0 || dest >= n) {
            printf("Invalid node indices\n");
            continue;
        }
        
        if (dist[src][dest] == INF) {
            printf("No path from %d to %d\n", src, dest);
        } else {
            printf("Shortest distance from %d to %d: %d\n", src, dest, dist[src][dest]);
        }
    }
    
    return 0;
}