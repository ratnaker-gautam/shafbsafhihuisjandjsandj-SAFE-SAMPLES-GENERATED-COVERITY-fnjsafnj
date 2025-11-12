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

void initializeGraph(struct Graph* graph, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        exit(1);
    }
    graph->numNodes = nodes;
    graph->numEdges = 0;
}

int addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (graph->numEdges >= MAX_EDGES) {
        return 0;
    }
    if (src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    
    graph->edges[graph->numEdges].src = src;
    graph->edges[graph->numEdges].dest = dest;
    graph->edges[graph->numEdges].weight = weight;
    graph->numEdges++;
    return 1;
}

void printGraph(struct Graph* graph) {
    printf("Graph with %d nodes and %d edges:\n", graph->numNodes, graph->numEdges);
    for (int i = 0; i < graph->numEdges; i++) {
        printf("Edge %d: %d -> %d (weight: %d)\n", 
               i, graph->edges[i].src, graph->edges[i].dest, graph->edges[i].weight);
    }
}

void dijkstra(struct Graph* graph, int start) {
    if (start < 0 || start >= graph->numNodes) {
        printf("Invalid start node\n");
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
        
        for (int v = 0; v < graph->numNodes; v++) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minNode = v;
            }
        }
        
        if (minNode == -1) break;
        visited[minNode] = 1;
        
        for (int i = 0; i < graph->numEdges; i++) {
            if (graph->edges[i].src == minNode) {
                int neighbor = graph->edges[i].dest;
                int newDist;
                if (dist[minNode] < INT_MAX && graph->edges[i].weight < INT_MAX - dist[minNode]) {
                    newDist = dist[minNode] + graph->edges[i].weight;
                } else {
                    newDist = INT_MAX;
                }
                if (!visited[neighbor] && newDist < dist[neighbor]) {
                    dist[neighbor] = newDist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
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
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &numNodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (numEdges < 0 || numEdges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!addEdge(&graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    printGraph(&graph);
    
    int startNode;
    printf("Enter start node for Dijkstra: ");
    if (scanf("%d", &startNode) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    dijkstra(&graph, startNode);
    
    return 0;
}