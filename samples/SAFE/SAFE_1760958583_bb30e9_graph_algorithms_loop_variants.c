//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
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
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->numNodes = nodes;
    graph->numEdges = edges;
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

void addEdge(struct Graph* graph, int index, int src, int dest, int weight) {
    if (index < 0 || index >= graph->numEdges) {
        fprintf(stderr, "Invalid edge index\n");
        exit(1);
    }
    if (!validateEdge(src, dest, weight, graph->numNodes)) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(1);
    }
    graph->edges[index].src = src;
    graph->edges[index].dest = dest;
    graph->edges[index].weight = weight;
}

void bellmanFord(struct Graph* graph, int source) {
    if (source < 0 || source >= graph->numNodes) {
        fprintf(stderr, "Invalid source node\n");
        return;
    }

    int distances[MAX_NODES];
    int i = 0;
    while (i < graph->numNodes) {
        distances[i] = INT_MAX;
        i++;
    }
    distances[source] = 0;

    int j;
    for (j = 0; j < graph->numNodes - 1; j++) {
        int updated = 0;
        int k = 0;
        do {
            int u = graph->edges[k].src;
            int v = graph->edges[k].dest;
            int weight = graph->edges[k].weight;
            if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                updated = 1;
            }
            k++;
        } while (k < graph->numEdges);
        if (!updated) break;
    }

    int negativeCycle = 0;
    int m = 0;
    while (m < graph->numEdges) {
        int u = graph->edges[m].src;
        int v = graph->edges[m].dest;
        int weight = graph->edges[m].weight;
        if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
            negativeCycle = 1;
            break;
        }
        m++;
    }

    if (negativeCycle) {
        printf("Graph contains negative weight cycle\n");
        return;
    }

    printf("Shortest distances from node %d:\n", source);
    int n = 0;
    for (n = 0; n < graph->numNodes; n++) {
        if (distances[n] == INT_MAX) {
            printf("Node %d: unreachable\n", n);
        } else {
            printf("Node %d: %d\n", n, distances[n]);
        }
    }
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &numNodes) != 1 || numNodes < 1 || numNodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-1000): ");
    if (scanf("%d", &numEdges) != 1 || numEdges < 0 || numEdges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes, numEdges);
    
    int i = 0;
    while (i < numEdges) {
        int src, dest, weight;
        printf("Enter edge %d (src dest weight): ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        addEdge(&graph, i, src, dest, weight);
        i++;
    }
    
    int source;
    printf("Enter source node: ");
    if (scanf("%d", &source) != 1 || source < 0 || source >= numNodes) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    bellmanFord(&graph, source);
    
    return 0;
}