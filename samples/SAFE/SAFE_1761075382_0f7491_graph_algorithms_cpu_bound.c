//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
}

int find_min_distance(int distances[], int visited[], int n) {
    int min = INT_MAX;
    int min_index = -1;
    for (int v = 0; v < n; v++) {
        if (visited[v] == 0 && distances[v] <= min) {
            min = distances[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(Graph *g, int source) {
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    distances[source] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int u = find_min_distance(distances, visited, g->node_count);
        if (u == -1) break;
        visited[u] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[u][v] != 0 && 
                distances[u] != INT_MAX) {
                int new_dist = distances[u] + g->adjacency[u][v];
                if (new_dist > 0 && new_dist < distances[v]) {
                    distances[v] = new_dist;
                }
            }
        }
    }
    
    printf("Vertex Distance from Source %d\n", source);
    for (int i = 0; i < g->node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("%d \t\t INF\n", i);
        } else {
            printf("%d \t\t %d\n", i, distances[i]);
        }
    }
}

int main() {
    Graph g;
    int nodes = 6;
    initialize_graph(&g, nodes);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 4);
    add_edge(&g, 1, 2, 2);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 3);
    add_edge(&g, 3, 4, 6);
    add_edge(&g, 3, 5, 7);
    add_edge(&g, 4, 5, 1);
    
    dijkstra(&g, 0);
    
    return 0;
}