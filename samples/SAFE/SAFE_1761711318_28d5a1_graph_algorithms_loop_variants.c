//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int num_nodes;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void init_graph(struct Graph *g, int n) {
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
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    g->adj_matrix[u][v] = weight;
    g->adj_matrix[v][u] = weight;
}

void dijkstra(struct Graph *g, int src) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        
        int i = 0;
        while (i < g->num_nodes) {
            if (!visited[i] && dist[i] <= min_dist) {
                min_dist = dist[i];
                min_index = i;
            }
            i++;
        }
        
        if (min_index == -1) break;
        
        visited[min_index] = 1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && g->adj_matrix[min_index][v] != 0) {
                long new_dist = (long)dist[min_index] + g->adj_matrix[min_index][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < g->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

void dfs_util(struct Graph *g, int node, int visited[]) {
    visited[node] = 1;
    printf("%d ", node);
    
    int v = 0;
    do {
        if (g->adj_matrix[node][v] != 0 && !visited[v]) {
            dfs_util(g, v, visited);
        }
        v++;
    } while (v < g->num_nodes);
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        visited[i] = 0;
    }
    
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    int num_nodes = 6;
    
    init_graph(&g, num_nodes);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 10);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 3, 5, 6);
    add_edge(&g, 4, 5, 3);
    
    printf("Graph with %d nodes created\n", num_nodes);
    
    int start_node;
    printf("Enter starting node for Dijkstra (0-%d): ", num_nodes - 1);
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (start_node < 0 || start_node >= num_nodes) {
        fprintf(stderr, "Invalid node\n");
        return 1;
    }
    
    dijkstra(&g, start_node);
    
    printf("Enter starting node for DFS (0-%d): ", num_nodes - 1);
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (start_node < 0 || start_node >= num_nodes) {
        fprintf(stderr, "Invalid node\n");
        return 1;
    }
    
    dfs(&g, start_node);
    
    return 0;
}