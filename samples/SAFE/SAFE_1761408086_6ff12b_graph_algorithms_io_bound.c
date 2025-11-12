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

int validateNode(int node, int maxNodes) {
    return node >= 0 && node < maxNodes;
}

int validateEdge(struct Edge edge, int maxNodes) {
    return validateNode(edge.src, maxNodes) && 
           validateNode(edge.dest, maxNodes) &&
           edge.weight >= 0 && edge.weight <= 1000;
}

void addEdge(struct Graph* graph, int index, int src, int dest, int weight) {
    if (index < 0 || index >= graph->numEdges) {
        printf("Invalid edge index\n");
        return;
    }
    struct Edge newEdge = {src, dest, weight};
    if (!validateEdge(newEdge, graph->numNodes)) {
        printf("Invalid edge parameters\n");
        return;
    }
    graph->edges[index] = newEdge;
}

void findShortestPaths(struct Graph* graph, int startNode) {
    if (!validateNode(startNode, graph->numNodes)) {
        printf("Invalid start node\n");
        return;
    }
    
    int distances[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[startNode] = 0;
    
    for (int i = 0; i < graph->numNodes - 1; i++) {
        for (int j = 0; j < graph->numEdges; j++) {
            struct Edge edge = graph->edges[j];
            if (edge.src == -1 || edge.dest == -1) continue;
            
            if (distances[edge.src] != INT_MAX) {
                long newDist = (long)distances[edge.src] + edge.weight;
                if (newDist < distances[edge.dest] && newDist <= INT_MAX) {
                    distances[edge.dest] = (int)newDist;
                }
            }
        }
    }
    
    printf("Shortest paths from node %d:\n", startNode);
    for (int i = 0; i < graph->numNodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &numNodes) != 1 || numNodes <= 0 || numNodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &numEdges) != 1 || numEdges < 0 || numEdges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes, numEdges);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        addEdge(&graph, i, src, dest, weight);
    }
    
    int startNode;
    printf("Enter start node for shortest paths: ");
    if (scanf("%d", &startNode) != 1) {
        printf("Invalid start node\n");
        return 1;
    }
    
    findShortestPaths(&graph, startNode);
    
    return 0;
}