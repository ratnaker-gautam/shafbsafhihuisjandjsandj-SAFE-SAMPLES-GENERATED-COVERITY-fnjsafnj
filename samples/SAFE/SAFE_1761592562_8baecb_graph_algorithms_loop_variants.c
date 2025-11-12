//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int nodes) {
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* g, int u, int v, int weight) {
    if (u >= 0 && u < g->nodes && v >= 0 && v < g->nodes) {
        g->adj[u][v] = weight;
        g->adj[v][u] = weight;
    }
}

int findMinDistance(int dist[], int visited[], int nodes) {
    int min = INT_MAX;
    int min_index = -1;
    
    for (int i = 0; i < nodes; i++) {
        if (!visited[i] && dist[i] <= min) {
            min = dist[i];
            min_index = i;
        }
    }
    return min_index;
}

void dijkstra(struct Graph* g, int src) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int u = findMinDistance(dist, visited, g->nodes);
        if (u == -1) break;
        
        visited[u] = 1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->adj[u][v] && dist[u] != INT_MAX) {
                int new_dist = dist[u] + g->adj[u][v];
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

void dfsUtil(struct Graph* g, int v, int visited[]) {
    visited[v] = 1;
    printf("%d ", v);
    
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[v][i] && !visited[i]) {
            dfsUtil(g, i, visited);
        }
    }
}

void dfs(struct Graph* g, int start) {
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfsUtil(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initializeGraph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes || weight < 0) {
            printf("Invalid edge parameters\n");
            return 1;
        }
        addEdge(&g, u, v, weight);
    }
    
    int start_node;
    printf("Enter starting node for algorithms: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    printf("\n");
    dijkstra(&g, start_node);
    printf("\n");
    dfs(&g, start_node);
    
    return 0;
}