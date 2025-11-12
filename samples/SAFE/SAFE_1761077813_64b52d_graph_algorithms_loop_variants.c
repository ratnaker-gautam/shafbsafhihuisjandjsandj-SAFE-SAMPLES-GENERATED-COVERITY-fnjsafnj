//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
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

void addEdge(struct Graph* g, int src, int dest, int weight) {
    if (src >= 0 && src < g->nodes && dest >= 0 && dest < g->nodes) {
        g->adj[src][dest] = weight;
        g->adj[dest][src] = weight;
    }
}

void dijkstra(struct Graph* g, int start) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start] = 0;

    for (int count = 0; count < g->nodes - 1; count++) {
        int minDist = INT_MAX;
        int minIndex = -1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minIndex = v;
            }
        }
        
        if (minIndex == -1) break;
        visited[minIndex] = 1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->adj[minIndex][v] && 
                dist[minIndex] != INT_MAX && 
                dist[minIndex] + g->adj[minIndex][v] < dist[v]) {
                dist[v] = dist[minIndex] + g->adj[minIndex][v];
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
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
    
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        printf("Enter edge %d (src dest weight): ", i + 1);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= nodes || dest < 0 || dest >= nodes || weight < 0) {
            printf("Invalid edge parameters\n");
            return 1;
        }
        addEdge(&g, src, dest, weight);
    }
    
    int startNode;
    printf("Enter starting node for Dijkstra: ");
    if (scanf("%d", &startNode) != 1 || startNode < 0 || startNode >= nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    dijkstra(&g, startNode);
    
    return 0;
}