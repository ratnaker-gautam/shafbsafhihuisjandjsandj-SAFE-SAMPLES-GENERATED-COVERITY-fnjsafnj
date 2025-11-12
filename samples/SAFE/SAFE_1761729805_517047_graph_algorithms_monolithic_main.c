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
        printf("Invalid graph parameters\n");
        exit(1);
    }
    graph->numNodes = nodes;
    graph->numEdges = edges;
}

int validateNode(struct Graph* graph, int node) {
    return node >= 0 && node < graph->numNodes;
}

void bellmanFord(struct Graph* graph, int source) {
    if (!validateNode(graph, source)) {
        printf("Invalid source node\n");
        return;
    }

    int dist[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[source] = 0;

    for (int i = 1; i <= graph->numNodes - 1; i++) {
        for (int j = 0; j < graph->numEdges; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int weight = graph->edges[j].weight;
            
            if (validateNode(graph, u) && validateNode(graph, v)) {
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                }
            }
        }
    }

    for (int i = 0; i < graph->numEdges; i++) {
        int u = graph->edges[i].src;
        int v = graph->edges[i].dest;
        int weight = graph->edges[i].weight;
        
        if (validateNode(graph, u) && validateNode(graph, v)) {
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                printf("Graph contains negative weight cycle\n");
                return;
            }
        }
    }

    printf("Shortest distances from node %d:\n", source);
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
    int numNodes, numEdges, source;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &numNodes) != 1 || numNodes < 1 || numNodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-1000): ");
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
        if (!validateNode(&graph, src) || !validateNode(&graph, dest)) {
            printf("Invalid node in edge\n");
            return 1;
        }
        graph.edges[i].src = src;
        graph.edges[i].dest = dest;
        graph.edges[i].weight = weight;
    }
    
    printf("Enter source node: ");
    if (scanf("%d", &source) != 1 || !validateNode(&graph, source)) {
        printf("Invalid source node\n");
        return 1;
    }
    
    bellmanFord(&graph, source);
    
    return 0;
}