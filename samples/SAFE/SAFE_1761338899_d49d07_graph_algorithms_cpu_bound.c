//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adj[MAX_NODES][MAX_NODES];
    int nodes;
} Graph;

void init_graph(Graph *g, int nodes) {
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

void add_edge(Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}

int min_distance(int dist[], int visited[], int n) {
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v < n; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(Graph *g, int src) {
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
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->adj[u][v] != 0 && dist[u] != INT_MAX) {
                long new_dist = (long)dist[u] + g->adj[u][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = new_dist;
                }
            }
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

void generate_random_graph(Graph *g, int edges) {
    if (edges < 0 || edges > g->nodes * (g->nodes - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        exit(1);
    }
    
    int added = 0;
    while (added < edges) {
        int u = rand() % g->nodes;
        int v = rand() % g->nodes;
        if (u != v && g->adj[u][v] == 0) {
            int weight = 1 + rand() % 20;
            add_edge(g, u, v, weight);
            added++;
        }
    }
}

int main() {
    srand(42);
    
    Graph g;
    int nodes = 8;
    int edges = 12;
    
    init_graph(&g, nodes);
    generate_random_graph(&g, edges);
    
    printf("Graph with %d nodes and %d edges:\n", nodes, edges);
    for (int i = 0; i < nodes; i++) {
        for (int j = i + 1; j < nodes; j++) {
            if (g.adj[i][j] != 0) {
                printf("Edge %d-%d: weight %d\n", i, j, g.adj[i][j]);
            }
        }
    }
    printf("\n");
    
    dijkstra(&g, 0);
    
    return 0;
}