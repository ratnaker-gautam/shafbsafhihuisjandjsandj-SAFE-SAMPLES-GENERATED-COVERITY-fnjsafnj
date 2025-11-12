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

int min_distance(int dist[], int visited[], int n) {
    int min = INT_MAX;
    int min_index = -1;
    for (int v = 0; v < n; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
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
        int u = min_distance(dist, visited, g->nodes);
        if (u == -1) break;
        visited[u] = 1;
        
        int v = 0;
        while (v < g->nodes) {
            if (!visited[v] && g->adj[u][v] && dist[u] != INT_MAX) {
                long new_dist = (long)dist[u] + g->adj[u][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
            v++;
        }
    }
    
    printf("Vertex Distance from Source %d\n", src);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d \t\t INF\n", i);
        } else {
            printf("%d \t\t %d\n", i, dist[i]);
        }
    }
}

void dfs_util(struct Graph *g, int v, int visited[]) {
    visited[v] = 1;
    printf("%d ", v);
    
    int i = 0;
    do {
        if (g->adj[v][i] && !visited[i]) {
            dfs_util(g, i, visited);
        }
        i++;
    } while (i < g->nodes);
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start vertex\n");
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        visited[i] = 0;
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
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * nodes) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes || w < 0) {
            fprintf(stderr, "Invalid edge parameters\n");
            return 1;
        }
        add_edge(&g, u, v, w);
    }
    
    int start_vertex;
    printf("Enter start vertex for Dijkstra: ");
    if (scanf("%d", &start_vertex) != 1 || start_vertex < 0 || start_vertex >= nodes) {
        fprintf(stderr, "Invalid start vertex\n");
        return 1;
    }
    
    dijkstra(&g, start_vertex);
    
    printf("Enter start vertex for DFS: ");
    if (scanf("%d", &start_vertex) != 1 || start_vertex < 0 || start