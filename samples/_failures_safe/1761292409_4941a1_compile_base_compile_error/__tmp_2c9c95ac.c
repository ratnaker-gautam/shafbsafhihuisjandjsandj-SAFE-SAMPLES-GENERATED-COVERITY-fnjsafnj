//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int numNodes;
    int numEdges;
    struct Edge edges[MAX_EDGES];
};

void initializeGraph(struct Graph* graph, int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return;
    }
    graph->numNodes = nodes;
    graph->numEdges = edges;
    for (int i = 0; i < edges; i++) {
        graph->edges[i].src = 0;
        graph->edges[i].dest = 0;
        graph->edges[i].weight = 0;
    }
}

int validateEdge(int src, int dest, int weight, int numNodes) {
    if (src < 0 || src >= numNodes || dest < 0 || dest >= numNodes) {
        return 0;
    }
    if (weight < -1000 || weight > 1000) {
        return 0;
    }
    return 1;
}

int addEdge(struct Graph* graph, int index, int src, int dest, int weight) {
    if (index < 0 || index >= graph->numEdges) {
        return 0;
    }
    if (!validateEdge(src, dest, weight, graph->numNodes)) {
        return 0;
    }
    graph->edges[index].src = src;
    graph->edges[index].dest = dest;
    graph->edges[index].weight = weight;
    return 1;
}

void bellmanFord(struct Graph* graph, int start) {
    if (graph == NULL || start < 0 || start >= graph->numNodes) {
        return;
    }
    
    int dist[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    for (int i = 1; i <= graph->numNodes - 1; i++) {
        for (int j = 0; j < graph->numEdges; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int weight = graph->edges[j].weight;
            
            if (dist[u] != INT_MAX) {
                long long newDist = (long long)dist[u] + weight;
                if (newDist < INT_MAX && newDist > INT_MIN) {
                    if (newDist < dist[v]) {
                        dist[v] = (int)newDist;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < graph->numEdges; i++) {
        int u = graph->edges[i].src;
        int v = graph->edges[i].dest;
        int weight = graph->edges[i].weight;
        
        if (dist[u] != INT_MAX) {
            long long newDist = (long long)dist[u] + weight;
            if (newDist < INT_MAX && newDist > INT_MIN) {
                if (newDist < dist[v]) {
                    printf("Graph contains negative weight cycle\n");
                    return;
                }
            }
        }
    }
    
    printf("Vertex distances from source %d:\n", start);
    for (int i = 0; i < graph->numNodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: INFINITY\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
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
    
    printf("Enter number of edges (0-1000): ");
    if (scanf("%d", &numEdges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (numNodes <= 0 || numNodes > MAX_NODES || numEdges < 0 || numEdges > MAX_EDGES) {
        printf("Invalid graph parameters\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes, numEdges);
    
    printf("Enter edges as 'source destination weight':\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        
        if (!addEdge(&graph, i, src, dest, weight)) {
            printf("Invalid edge parameters\n");
            return 1;
        }
    }
    
    int startNode;
    printf("Enter starting node: ");
    if (scanf("%d", &startNode) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (startNode < 0 || startNode >= numNodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    bellmanFord(&graph, startNode);
    return