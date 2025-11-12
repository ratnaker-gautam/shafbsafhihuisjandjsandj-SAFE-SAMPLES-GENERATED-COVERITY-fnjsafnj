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

void initializeGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->matrix[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int addEdge(struct Graph* g, int from, int to, int weight) {
    if (g == NULL || from < 0 || from >= g->nodes || to < 0 || to >= g->nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    g->matrix[from][to] = weight;
    g->matrix[to][from] = weight;
    return 1;
}

void dijkstra(struct Graph* g, int start, int distances[]) {
    if (g == NULL || distances == NULL || start < 0 || start >= g->nodes) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->nodes; i++) {
        distances[i] = INF;
    }
    distances[start] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int minDist = INF;
        int minNode = -1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && distances[v] <= minDist) {
                minDist = distances[v];
                minNode = v;
            }
        }
        
        if (minNode == -1) break;
        visited[minNode] = 1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->matrix[minNode][v] != INF && 
                distances[minNode] != INF) {
                long newDist = (long)distances[minNode] + g->matrix[minNode][v];
                if (newDist < distances[v] && newDist <= INT_MAX) {
                    distances[v] = (int)newDist;
                }
            }
        }
    }
}

void printDistances(struct Graph* g, int start, int distances[]) {
    if (g == NULL || distances == NULL || start < 0 || start >= g->nodes) {
        return;
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < g->nodes; i++) {
        if (distances[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int distances[MAX_NODES];
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initializeGraph(&graph, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (from to weight):\n");
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!addEdge(&graph, from, to, weight)) {
            printf("Invalid edge: %d %d %d\n", from, to, weight);
            return 1;
        }
    }
    
    int startNode;
    printf("Enter starting node: ");
    if (scanf("%d", &startNode) != 1 || startNode < 0 || startNode >= nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    dijkstra(&graph, startNode, distances);
    printDistances(&graph, startNode, distances);
    
    return 0;
}