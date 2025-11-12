//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int matrix[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->matrix[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void addEdge(struct Graph* g, int src, int dest, int weight) {
    if (src < 0 || src >= g->nodes || dest < 0 || dest >= g->nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    g->matrix[src][dest] = weight;
    g->matrix[dest][src] = weight;
}

void dijkstra(struct Graph* g, int start) {
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int minDist = INF;
        int minNode = -1;
        
        for (int i = 0; i < g->nodes; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minNode = i;
            }
        }
        
        if (minNode == -1) break;
        visited[minNode] = 1;
        
        for (int i = 0; i < g->nodes; i++) {
            if (!visited[i] && g->matrix[minNode][i] != INF && dist[minNode] != INF) {
                long newDist = (long)dist[minNode] + g->matrix[minNode][i];
                if (newDist < dist[i] && newDist <= INT_MAX) {
                    dist[i] = (int)newDist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INF) {
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
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initializeGraph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        addEdge(&g, src, dest, weight);
    }
    
    int startNode;
    printf("Enter starting node for Dijkstra: ");
    if (scanf("%d", &startNode) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (startNode < 0 || startNode >= nodes) {
        fprintf(stderr, "Invalid start node\n");
        return 1;
    }
    
    dijkstra(&g, startNode);
    
    return 0;
}