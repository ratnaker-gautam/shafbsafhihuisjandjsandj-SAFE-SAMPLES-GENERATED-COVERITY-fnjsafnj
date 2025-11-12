//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

void init_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}

void dijkstra(struct Graph *g, int src) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        
        int i = 0;
        while (i < g->nodes) {
            if (!visited[i] && dist[i] <= min_dist) {
                min_dist = dist[i];
                min_index = i;
            }
            i++;
        }
        
        if (min_index == -1) break;
        
        visited[min_index] = 1;
        
        int v = 0;
        do {
            if (!visited[v] && g->adj[min_index][v] != 0 && 
                dist[min_index] != INT_MAX && 
                dist[min_index] + g->adj[min_index][v] < dist[v]) {
                dist[v] = dist[min_index] + g->adj[min_index][v];
            }
            v++;
        } while (v < g->nodes);
    }
    
    printf("Vertex Distance from Source %d\n", src);
    for (int j = 0; j < g->nodes; j++) {
        if (dist[j] == INT_MAX) {
            printf("%d \t\t INF\n", j);
        } else {
            printf("%d \t\t %d\n", j, dist[j]);
        }
    }
}

void dfs_util(struct Graph *g, int v, int visited[]) {
    visited[v] = 1;
    printf("%d ", v);
    
    int w = 0;
    for (; w < g->nodes; w++) {
        if (g->adj[v][w] != 0 && !visited[w]) {
            dfs_util(g, w, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start vertex\n");
        return;
    }
    
    int visited[MAX_NODES];
    int i = 0;
    while (i < g->nodes) {
        visited[i] = 0;
        i++;
    }
    
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    init_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int e = 0; e < edges; e++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes || weight < 0) {
            fprintf(stderr, "Invalid edge parameters\n");
            return 1;
        }
        add_edge(&g, u, v, weight);
    }
    
    int start_vertex;
    printf("Enter start vertex for algorithms: ");
    if (scanf("%d", &start_vertex) != 1 || start_vertex < 0 || start_vertex >= nodes) {
        fprintf(stderr, "Invalid start vertex\n");
        return 1;
    }
    
    printf("\n");
    dijkstra(&g, start_vertex);
    printf("\n");
    dfs(&g, start_vertex);
    
    return 0;
}