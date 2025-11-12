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
            if (!visited[v] && g->adj[min_index][v] && 
                dist[min_index] != INT_MAX &&
                dist[min_index] + g->adj[min_index][v] < dist[v]) {
                dist[v] = dist[min_index] + g->adj[min_index][v];
            }
            v++;
        } while (v < g->nodes);
    }
    
    printf("Vertex distances from source %d:\n", src);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d: INF\n", i);
        } else {
            printf("%d: %d\n", i, dist[i]);
        }
    }
}

void bfs(struct Graph *g, int start) {
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal: ");
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->nodes; i++) {
            if (g->adj[current][i] && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    init_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * nodes) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter edges as u v weight (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes) {
            fprintf(stderr, "Invalid node index\n");
            return 1;
        }
        add_edge(&g, u, v, w);
    }
    
    int source;
    printf("Enter source for Dijkstra: ");
    if (scanf("%d", &source) != 1 || source < 0 || source >= nodes) {
        fprintf(stderr, "Invalid source\n");
        return 1;
    }
    
    dijkstra(&g, source);
    
    int bfs_start;
    printf("Enter start for BFS: ");
    if (scanf("%d", &bfs_start) != 1 || bfs_start < 0 || bfs_start >= nodes) {
        fprintf(stderr, "Invalid start node\n");
        return 1;
    }
    
    bfs(&g, bfs_start);