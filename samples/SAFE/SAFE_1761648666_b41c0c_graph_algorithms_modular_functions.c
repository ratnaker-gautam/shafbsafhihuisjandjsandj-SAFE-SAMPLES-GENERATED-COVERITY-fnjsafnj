//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int matrix[MAX_NODES][MAX_NODES];
};

struct Graph create_graph(int nodes) {
    struct Graph g;
    if (nodes <= 0 || nodes > MAX_NODES) {
        g.nodes = 0;
        return g;
    }
    g.nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g.matrix[i][j] = (i == j) ? 0 : INF;
        }
    }
    return g;
}

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes || weight < 0) {
        return 0;
    }
    g->matrix[u][v] = weight;
    g->matrix[v][u] = weight;
    return 1;
}

void dijkstra(struct Graph *g, int source, int dist[]) {
    if (g == NULL || source < 0 || source >= g->nodes || dist == NULL) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
    }
    dist[source] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int min_dist = INF;
        int min_node = -1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->matrix[min_node][v] != INF && dist[min_node] != INF) {
                long new_dist = (long)dist[min_node] + g->matrix[min_node][v];
                if (new_dist < dist[v] && new_dist < INF) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph *g, int source) {
    if (g == NULL || source < 0 || source >= g->nodes) {
        return;
    }
    
    int dist[MAX_NODES];
    dijkstra(g, source, dist);
    
    printf("Shortest paths from node %d:\n", source);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g = create_graph(6);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 10);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 3, 5, 6);
    add_edge(&g, 4, 5, 3);
    
    printf("Graph with %d nodes created.\n", g.nodes);
    printf("Performing Dijkstra's algorithm...\n\n");
    
    for (int source = 0; source < g.nodes; source++) {
        print_shortest_paths(&g, source);
        printf("\n");
    }
    
    return 0;
}