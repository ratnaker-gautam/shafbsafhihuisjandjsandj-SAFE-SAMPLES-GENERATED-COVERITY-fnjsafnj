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
        fprintf(stderr, "Invalid edge parameters\n");
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
        return;
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
                if (dist[minNode] != INT_MAX) {
                    int newDist = dist[minNode] + graph->adjMatrix[minNode][j];
                    if (newDist > 0 && newDist < dist[j]) {
                        dist[j] = newDist;
                    }
                }
            }
            j++;
        } while (j < graph->numNodes);
    }
    
    printf("Shortest distances from node %d:\n", startNode);
    for (int k = 0; k < graph->numNodes; k++) {
        if (dist[k] == INT_MAX) {
            printf("Node %d: unreachable\n", k);
        } else {
            printf("Node %d: %d\n", k, dist[k]);
        }
    }
}

void printGraph(struct Graph* graph) {
    printf("Graph adjacency matrix:\n");
    for (int i = 0; i < graph->numNodes; i++) {
        for (int j = 0; j < graph->numNodes; j++) {
            printf("%3d ", graph->adjMatrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &numNodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (numNodes <= 0 || numNodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (numEdges < 0 || numEdges > numNodes * (numNodes - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    for (int edgeCount = 0; edgeCount < numEdges; edgeCount++) {
        int src, dest, weight;
        printf("Enter edge %d (src dest weight): ", edgeCount + 1);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        
        if (src < 0 || src >= numNodes || dest < 0 || dest >= numNodes) {
            fprintf(stderr, "Invalid node indices\n");
            return 1;
        }
        
        if (weight < 0) {
            fprintf(stderr, "Negative weights not allowed\n");
            return 1;
        }
        
        addEdge(&graph, src, dest, weight);
    }
    
    printGraph(&graph);
    
    int startNode;
    printf("Enter start node for Dijkstra: ");
    if (scanf("%d", &startNode)