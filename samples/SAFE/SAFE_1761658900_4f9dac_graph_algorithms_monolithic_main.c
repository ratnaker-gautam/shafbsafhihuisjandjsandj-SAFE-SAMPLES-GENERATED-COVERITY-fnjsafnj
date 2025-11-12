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

int validateNode(int node, int maxNodes) {
    return node >= 0 && node < maxNodes;
}

void bellmanFord(struct Graph* graph, int source) {
    if (!validateNode(source, graph->numNodes)) {
        printf("Invalid source node\n");
        return;
    }

    int distances[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;

    for (int i = 1; i <= graph->numNodes - 1; i++) {
        for (int j = 0; j < graph->numEdges; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int weight = graph->edges[j].weight;
            
            if (validateNode(u, graph->numNodes) && validateNode(v, graph->numNodes)) {
                if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                }
            }
        }
    }

    for (int i = 0; i < graph->numEdges; i++) {
        int u = graph->edges[i].src;
        int v = graph->edges[i].dest;
        int weight = graph->edges[i].weight;
        
        if (validateNode(u, graph->numNodes) && validateNode(v, graph->numNodes)) {
            if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                printf("Graph contains negative weight cycle\n");
                return;
            }
        }
    }

    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < graph->numNodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: INFINITY\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
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
    
    printf("Enter edges as 'source destination weight':\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!validateNode(src, numNodes) || !validateNode(dest, numNodes)) {
            printf("Invalid node in edge\n");
            return 1;
        }
        graph.edges[i].src = src;
        graph.edges[i].dest = dest;
        graph.edges[i].weight = weight;
    }
    
    int source;
    printf("Enter source node: ");
    if (scanf("%d", &source) != 1 || !validateNode(source, numNodes)) {
        printf("Invalid source node\n");
        return 1;
    }
    
    bellmanFord(&graph, source);
    
    return 0;
}