//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int numNodes;
    int adjMatrix[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* graph, int numNodes) {
    if (numNodes <= 0 || numNodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    
    graph->numNodes = numNodes;
    
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        fprintf(stderr, "Invalid node indices\n");
        return;
    }
    
    if (weight < 0) {
        fprintf(stderr, "Weight must be non-negative\n");
        return;
    }
    
    graph->adjMatrix[src][dest] = weight;
    graph->adjMatrix[dest][src] = weight;
}

void dijkstra(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->numNodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->numNodes; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    distances[startNode] = 0;
    
    int count = 0;
    while (count < graph->numNodes) {
        int minDistance = INT_MAX;
        int currentNode = -1;
        
        for (int i = 0; i < graph->numNodes; i++) {
            if (!visited[i] && distances[i] < minDistance) {
                minDistance = distances[i];
                currentNode = i;
            }
        }
        
        if (currentNode == -1) break;
        
        visited[currentNode] = 1;
        count++;
        
        for (int neighbor = 0; neighbor < graph->numNodes; neighbor++) {
            if (graph->adjMatrix[currentNode][neighbor] > 0 && !visited[neighbor]) {
                int newDistance = distances[currentNode] + graph->adjMatrix[currentNode][neighbor];
                if (newDistance > 0 && newDistance < distances[neighbor]) {
                    distances[neighbor] = newDistance;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", startNode);
    for (int i = 0; i < graph->numNodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

void depthFirstSearch(struct Graph* graph, int startNode, int* visited) {
    if (startNode < 0 || startNode >= graph->numNodes) {
        return;
    }
    
    visited[startNode] = 1;
    printf("%d ", startNode);
    
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[startNode][i] > 0 && !visited[i]) {
            depthFirstSearch(graph, i, visited);
        }
    }
}

void performDFS(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->numNodes) {
        fprintf(stderr, "Invalid start node for DFS\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from node %d: ", startNode);
    depthFirstSearch(graph, startNode, visited);
    printf("\n");
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &numNodes) != 1 || numNodes <= 0 || numNodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1 || numEdges < 0 || numEdges > numNodes * (numNodes - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    for (int edgeCount = 0; edgeCount < numEdges; edgeCount++) {
        int src, dest, weight;
        printf("Enter edge %d (source destination weight): ", edgeCount + 1);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        addEdge(&graph, src, dest, weight);
    }
    
    int startNode;
    printf("Enter start node for Dijkstra's algorithm: ");
    if (scanf("%d", &startNode) != 1