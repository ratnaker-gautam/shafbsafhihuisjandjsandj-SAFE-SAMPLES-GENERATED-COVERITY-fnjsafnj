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

void dijkstra(struct Graph *g, int src) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    int count = 0;
    while (count < g->num_nodes) {
        int min_dist = INT_MAX;
        int u = -1;
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }
        
        if (u == -1) break;
        
        visited[u] = 1;
        count++;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && g->adj_matrix[u][v] != 0) {
                int new_dist;
                if (dist[u] < INT_MAX - g->adj_matrix[u][v]) {
                    new_dist = dist[u] + g->adj_matrix[u][v];
                } else {
                    new_dist = INT_MAX;
                }
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
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

void dfs_util(struct Graph *g, int v, int visited[]) {
    visited[v] = 1;
    printf("%d ", v);
    
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[v][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfs_util(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    int num_nodes = 6;
    
    initialize_graph(&g, num_nodes);
    
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
    printf("\n");
    
    printf("Running BFS-like traversal:\n");
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int visited[MAX_NODES] = {0};
    
    visited[0] = 1;
    queue[rear++] = 0;
    
    printf("BFS traversal: ");
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g.num_nodes; i++) {
            if (g.adj_matrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
    
    return 0;
}