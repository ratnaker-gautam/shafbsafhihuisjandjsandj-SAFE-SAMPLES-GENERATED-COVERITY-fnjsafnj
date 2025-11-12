//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int num_nodes;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g, int n) {
    if (n <= 0 || n > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->adj_matrix[i][j] = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adj_matrix[u][v] = weight;
    g->adj_matrix[v][u] = weight;
}

int find_min_distance(int dist[], int visited[], int n) {
    int min = INT_MAX;
    int min_index = -1;
    int i = 0;
    while (i < n) {
        if (!visited[i] && dist[i] <= min) {
            min = dist[i];
            min_index = i;
        }
        i++;
    }
    return min_index;
}

void dijkstra(struct Graph *g, int src) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    int count = 0;
    do {
        int u = find_min_distance(dist, visited, g->num_nodes);
        if (u == -1) break;
        
        visited[u] = 1;
        
        int v = 0;
        while (v < g->num_nodes) {
            if (!visited[v] && g->adj_matrix[u][v] && dist[u] != INT_MAX) {
                int new_dist = dist[u] + g->adj_matrix[u][v];
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
            }
            v++;
        }
        count++;
    } while (count < g->num_nodes);
    
    printf("Vertex distances from source %d:\n", src);
    for (int i = 0; i < g->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d: INF\n", i);
        } else {
            printf("%d: %d\n", i, dist[i]);
        }
    }
}

void dfs_util(struct Graph *g, int v, int visited[]) {
    visited[v] = 1;
    printf("%d ", v);
    
    int i = 0;
    for (; i < g->num_nodes; i++) {
        if (g->adj_matrix[v][i] && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid start vertex\n");
        return;
    }
    
    int visited[MAX_NODES];
    int i = 0;
    while (i < g->num_nodes) {
        visited[i] = 0;
        i++;
    }
    
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    initialize_graph(&g, 6);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 10);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 3, 5, 6);
    add_edge(&g, 4, 5, 3);
    
    printf("Graph algorithms demonstration:\n\n");
    
    dijkstra(&g, 0);
    printf("\n");
    
    dfs(&g, 0);
    
    return 0;
}