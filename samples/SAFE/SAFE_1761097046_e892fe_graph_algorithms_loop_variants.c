//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int numNodes;
    int adjMatrix[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int n) {
    if (n <= 0 || n > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->numNodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* g, int u, int v, int weight) {
    if (u < 0 || u >= g->numNodes || v < 0 || v >= g->numNodes) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adjMatrix[u][v] = weight;
    g->adjMatrix[v][u] = weight;
}

void dijkstra(struct Graph* g, int src) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->numNodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    int count = 0;
    while (count < g->numNodes) {
        int minDist = INT_MAX;
        int minNode = -1;
        
        for (int i = 0; i < g->numNodes; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minNode = i;
            }
        }
        
        if (minNode == -1) break;
        
        visited[minNode] = 1;
        count++;
        
        for (int v = 0; v < g->numNodes; v++) {
            if (!visited[v] && g->adjMatrix[minNode][v] != 0) {
                int newDist;
                if (dist[minNode] < INT_MAX - g->adjMatrix[minNode][v]) {
                    newDist = dist[minNode] + g->adjMatrix[minNode][v];
                } else {
                    newDist = INT_MAX;
                }
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < g->numNodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &numNodes) != 1 || numNodes < 1 || numNodes > MAX_NODES) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initializeGraph(&g, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1 || numEdges < 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    for (int i = 0; i < numEdges; i++) {
        int u, v, w;
        printf("Enter edge %d (u v weight): ", i + 1);
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }
        if (u < 0 || u >= numNodes || v < 0 || v >= numNodes || w < 0) {
            fprintf(stderr, "Invalid edge parameters\n");
            return 1;
        }
        addEdge(&g, u, v, w);
    }
    
    int source;
    printf("Enter source node for Dijkstra: ");
    if (scanf("%d", &source) != 1 || source < 0 || source >= numNodes) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    dijkstra(&g, source);
    
    return 0;
}