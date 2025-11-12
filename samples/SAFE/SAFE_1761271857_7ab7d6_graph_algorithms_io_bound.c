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

void addEdge(struct Graph* graph, int idx, int src, int dest, int weight) {
    if (idx < 0 || idx >= graph->numEdges) {
        printf("Invalid edge index\n");
        return;
    }
    struct Edge newEdge = {src, dest, weight};
    if (!validateEdge(newEdge, graph->numNodes)) {
        printf("Invalid edge parameters\n");
        return;
    }
    graph->edges[idx] = newEdge;
}

void dijkstra(struct Graph* graph, int startNode) {
    if (!validateNode(startNode, graph->numNodes)) {
        printf("Invalid start node\n");
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

        for (int v = 0; v < graph->numNodes; v++) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minNode = v;
            }
        }

        if (minNode == -1) break;

        visited[minNode] = 1;

        for (int i = 0; i < graph->numEdges; i++) {
            struct Edge edge = graph->edges[i];
            if (edge.src == minNode && !visited[edge.dest] && edge.weight > 0) {
                if (dist[minNode] != INT_MAX) {
                    long newDist = (long)dist[minNode] + edge.weight;
                    if (newDist < INT_MAX && newDist < dist[edge.dest]) {
                        dist[edge.dest] = (int)newDist;
                    }
                }
            }
        }
    }

    printf("Shortest distances from node %d:\n", startNode);
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

    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &numEdges) != 1) {
        printf("Invalid input\n");
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
    printf("Enter starting node for Dijkstra: ");
    if (scanf("%d", &startNode) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    dijkstra(&graph, startNode);

    return 0;
}