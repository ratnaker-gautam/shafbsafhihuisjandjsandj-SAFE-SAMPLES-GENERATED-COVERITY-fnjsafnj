//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int numNodes;
    int adjMatrix[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* graph, int nodes) {
    graph->numNodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (src >= 0 && src < graph->numNodes && dest >= 0 && dest < graph->numNodes) {
        graph->adjMatrix[src][dest] = weight;
        graph->adjMatrix[dest][src] = weight;
    }
}

void dijkstra(struct Graph* graph, int startNode) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[startNode] = 0;
    
    for (int count = 0; count < graph->numNodes - 1; count++) {
        int minDist = INT_MAX;
        int minIndex = -1;
        
        for (int v = 0; v < graph->numNodes; v++) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minIndex = v;
            }
        }
        
        if (minIndex == -1) break;
        visited[minIndex] = 1;
        
        for (int v = 0; v < graph->numNodes; v++) {
            if (!visited[v] && graph->adjMatrix[minIndex][v] && 
                dist[minIndex] != INT_MAX && 
                dist[minIndex] + graph->adjMatrix[minIndex][v] < dist[v]) {
                dist[v] = dist[minIndex] + graph->adjMatrix[minIndex][v];
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", startNode);
    for (int i = 0; i < graph->numNodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    if (scanf("%d", &numNodes) != 1 || numNodes <= 0 || numNodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1 || numEdges < 0 || numEdges > numNodes * (numNodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    for (int edgeCount = 0; edgeCount < numEdges; edgeCount++) {
        int src, dest, weight;
        printf("Enter edge %d (source destination weight): ", edgeCount + 1);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= numNodes || dest < 0 || dest >= numNodes || weight <= 0) {
            printf("Invalid edge parameters\n");
            return 1;
        }
        addEdge(&graph, src, dest, weight);
    }
    
    int startNode;
    printf("Enter starting node for Dijkstra: ");
    if (scanf("%d", &startNode) != 1 || startNode < 0 || startNode >= numNodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    dijkstra(&graph, startNode);
    
    return 0;
}