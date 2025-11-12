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
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    
    graph->numNodes = nodes;
    
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        fprintf(stderr, "Invalid node indices\n");
        exit(1);
    }
    
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    
    graph->adjMatrix[src][dest] = weight;
    graph->adjMatrix[dest][src] = weight;
}

void dijkstra(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->numNodes) {
        fprintf(stderr, "Invalid start node\n");
        exit(1);
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[startNode] = 0;
    
    for (int count = 0; count < graph->numNodes - 1; count++) {
        int minDist = INT_MAX;
        int minNode = -1;
        
        int i = 0;
        while (i < graph->numNodes) {
            if (!visited[i] && dist[i] <= minDist) {
                minDist = dist[i];
                minNode = i;
            }
            i++;
        }
        
        if (minNode == -1) break;
        
        visited[minNode] = 1;
        
        int j = 0;
        do {
            if (!visited[j] && graph->adjMatrix[minNode][j] != 0) {
                long newDist = (long)dist[minNode] + graph->adjMatrix[minNode][j];
                if (newDist < INT_MAX && newDist < dist[j]) {
                    dist[j] = (int)newDist;
                }
            }
            j++;
        } while (j < graph->numNodes);
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
    int numNodes = 6;
    
    initializeGraph(&graph, numNodes);
    
    addEdge(&graph, 0, 1, 4);
    addEdge(&graph, 0, 2, 2);
    addEdge(&graph, 1, 2, 1);
    addEdge(&graph, 1, 3, 5);
    addEdge(&graph, 2, 3, 8);
    addEdge(&graph, 2, 4, 10);
    addEdge(&graph, 3, 4, 2);
    addEdge(&graph, 3, 5, 6);
    addEdge(&graph, 4, 5, 3);
    
    printf("Graph with %d nodes created\n", numNodes);
    printf("Running Dijkstra's algorithm from node 0:\n");
    
    dijkstra(&graph, 0);
    
    return 0;
}