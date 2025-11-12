//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
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
    if (graph == NULL || index < 0 || index >= graph->numEdges) {
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

void bellmanFord(struct Graph* graph, int source) {
    if (graph == NULL || source < 0 || source >= graph->numNodes) {
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
            
            if (distances[u] != INT_MAX) {
                long long newDist = (long long)distances[u] + weight;
                if (newDist < distances[v] && newDist < INT_MAX) {
                    distances[v] = (int)newDist;
                }
            }
        }
    }
    
    for (int i = 0; i < graph->numEdges; i++) {
        int u = graph->edges[i].src;
        int v = graph->edges[i].dest;
        int weight = graph->edges[i].weight;
        if (distances[u] != INT_MAX && (long long)distances[u] + weight < distances[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Vertex distances from source %d:\n", source);
    for (int i = 0; i < graph->numNodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: INFINITY\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int readInt(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph graph;
    
    printf("Graph Algorithm: Bellman-Ford Shortest Path\n");
    
    int nodes = readInt("Enter number of nodes (1-100): ", 1, MAX_NODES);
    int edges = readInt("Enter number of edges (0-1000): ", 0, MAX_EDGES);
    
    initializeGraph(&graph, nodes, edges);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < edges; i++) {
        printf("Edge %d: ", i + 1);
        
        int src, dest, weight;
        char buffer[100];
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            src = 0;
            dest = 0;
            weight = 0;
        } else {
            if (sscanf(buffer, "%d %d %d", &src, &dest, &weight) != 3) {
                printf("Invalid edge format. Using default 0 0 0.\n");
                src = 0;
                dest = 0;
                weight = 0;
            }
        }
        
        if (!addEdge(&graph, i, src, dest, weight)) {
            printf("Invalid