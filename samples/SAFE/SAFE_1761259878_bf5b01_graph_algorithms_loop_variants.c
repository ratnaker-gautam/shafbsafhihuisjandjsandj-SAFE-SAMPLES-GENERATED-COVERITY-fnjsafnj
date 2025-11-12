//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int numNodes;
    int adjMatrix[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        exit(1);
    }
    
    g->numNodes = nodes;
    
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* g, int src, int dest, int weight) {
    if (src < 0 || src >= g->numNodes || dest < 0 || dest >= g->numNodes) {
        printf("Invalid node indices\n");
        return;
    }
    
    if (weight < 0) {
        printf("Weight must be non-negative\n");
        return;
    }
    
    g->adjMatrix[src][dest] = weight;
    g->adjMatrix[dest][src] = weight;
}

void dijkstra(struct Graph* g, int startNode) {
    if (startNode < 0 || startNode >= g->numNodes) {
        printf("Invalid start node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->numNodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    
    dist[startNode] = 0;
    
    int count = 0;
    while (count < g->numNodes) {
        int minDist = INT_MAX;
        int minNode = -1;
        
        int j = 0;
        while (j < g->numNodes) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                minNode = j;
            }
            j++;
        }
        
        if (minNode == -1) break;
        
        visited[minNode] = 1;
        count++;
        
        int k = 0;
        do {
            if (!visited[k] && g->adjMatrix[minNode][k] != 0) {
                int newDist;
                if (dist[minNode] < INT_MAX - g->adjMatrix[minNode][k]) {
                    newDist = dist[minNode] + g->adjMatrix[minNode][k];
                } else {
                    newDist = INT_MAX;
                }
                
                if (newDist < dist[k]) {
                    dist[k] = newDist;
                }
            }
            k++;
        } while (k < g->numNodes);
    }
    
    printf("Shortest distances from node %d:\n", startNode);
    for (int i = 0; i < g->numNodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

void printGraph(struct Graph* g) {
    printf("Graph adjacency matrix:\n");
    for (int i = 0; i < g->numNodes; i++) {
        for (int j = 0; j < g->numNodes; j++) {
            printf("%3d ", g->adjMatrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &numNodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (numEdges < 0 || numEdges > numNodes * (numNodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'source destination weight':\n");
    for (int edgeCount = 0; edgeCount < numEdges; edgeCount++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        addEdge(&graph, src, dest, weight);
    }
    
    printGraph(&graph);
    
    int startNode;
    printf("Enter start node for Dijkstra: ");
    if (scanf("%d", &startNode) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    dijkstra(&graph, startNode);
    
    return 0;
}