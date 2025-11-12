//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        printf("Invalid graph parameters\n");
        exit(1);
    }
    graph->numNodes = nodes;
    graph->numEdges = edges;
    for (int i = 0; i < edges; i++) {
        graph->edges[i].src = -1;
        graph->edges[i].dest = -1;
        graph->edges[i].weight = 0;
    }
}

int validateNode(struct Graph* graph, int node) {
    return node >= 0 && node < graph->numNodes;
}

int addEdge(struct Graph* graph, int edgeIndex, int src, int dest, int weight) {
    if (edgeIndex < 0 || edgeIndex >= graph->numEdges) {
        return 0;
    }
    if (!validateNode(graph, src) || !validateNode(graph, dest)) {
        return 0;
    }
    if (src == dest) {
        return 0;
    }
    if (weight < -1000 || weight > 1000) {
        return 0;
    }
    graph->edges[edgeIndex].src = src;
    graph->edges[edgeIndex].dest = dest;
    graph->edges[edgeIndex].weight = weight;
    return 1;
}

void printGraph(struct Graph* graph) {
    printf("Graph with %d nodes and %d edges:\n", graph->numNodes, graph->numEdges);
    for (int i = 0; i < graph->numEdges; i++) {
        if (graph->edges[i].src != -1 && graph->edges[i].dest != -1) {
            printf("Edge %d: %d -> %d (weight: %d)\n", 
                   i, graph->edges[i].src, graph->edges[i].dest, graph->edges[i].weight);
        }
    }
}

void findShortestPath(struct Graph* graph, int start, int end) {
    if (!validateNode(graph, start) || !validateNode(graph, end)) {
        printf("Invalid start or end node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    for (int count = 0; count < graph->numNodes - 1; count++) {
        int minDist = INT_MAX;
        int minNode = -1;
        
        for (int i = 0; i < graph->numNodes; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minNode = i;
            }
        }
        
        if (minNode == -1) break;
        visited[minNode] = 1;
        
        for (int i = 0; i < graph->numEdges; i++) {
            if (graph->edges[i].src == minNode && graph->edges[i].src != -1) {
                int dest = graph->edges[i].dest;
                int weight = graph->edges[i].weight;
                if (!visited[dest] && dist[minNode] != INT_MAX) {
                    long newDist = (long)dist[minNode] + weight;
                    if (newDist < INT_MAX && newDist > INT_MIN) {
                        if (newDist < dist[dest]) {
                            dist[dest] = (int)newDist;
                        }
                    }
                }
            }
        }
    }
    
    if (dist[end] == INT_MAX) {
        printf("No path exists from node %d to node %d\n", start, end);
    } else {
        printf("Shortest distance from node %d to node %d: %d\n", start, end, dist[end]);
    }
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &numNodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &numEdges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes, numEdges);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!addEdge(&graph, i, src, dest, weight)) {
            printf("Invalid edge