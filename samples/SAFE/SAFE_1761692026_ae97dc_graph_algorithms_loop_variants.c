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
        numNodes = MAX_NODES;
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
        return;
    }
    graph->adjMatrix[src][dest] = weight;
    graph->adjMatrix[dest][src] = weight;
}

void dijkstra(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->numNodes) {
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
        
        int i = 0;
        while (i < graph->numNodes) {
            if (!visited[i] && dist[i] <= minDist) {
                minDist = dist[i];
                minNode = i;
            }
            i++;
        }
        
        if (minNode == -1) break;
        visited[minNode] = 1;
        
        int j = 0;
        do {
            if (!visited[j] && graph->adjMatrix[minNode][j] != 0) {
                int newDist = dist[minNode] + graph->adjMatrix[minNode][j];
                if (newDist > 0 && newDist < dist[j]) {
                    dist[j] = newDist;
                }
            }
            j++;
        } while (j < graph->numNodes);
    }
    
    printf("Shortest distances from node %d:\n", startNode);
    for (int k = 0; k < graph->numNodes; k++) {
        if (dist[k] == INT_MAX) {
            printf("Node %d: unreachable\n", k);
        } else {
            printf("Node %d: %d\n", k, dist[k]);
        }
    }
}

void dfsUtil(struct Graph* graph, int node, int visited[]) {
    visited[node] = 1;
    printf("%d ", node);
    
    int i = 0;
    while (i < graph->numNodes) {
        if (graph->adjMatrix[node][i] != 0 && !visited[i]) {
            dfsUtil(graph, i, visited);
        }
        i++;
    }
}

void dfs(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->numNodes) {
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        visited[i] = 0;
    }
    
    printf("DFS traversal from node %d: ", startNode);
    dfsUtil(graph, startNode, visited);
    printf("\n");
}

int main() {
    struct Graph graph;
    int numNodes = 6;
    
    initializeGraph(&graph, numNodes);
    
    addEdge(&graph, 0, 1, 4);
    addEdge(&graph, 0, 2, 2);
    addEdge(&graph, 1, 2, 1);
    addEdge(&graph, 1, 3, 5);
    addEdge(&graph, 2, 3, 8);
    addEdge(&graph, 2, 4, 10);
    addEdge(&graph, 3, 4, 2);
    addEdge(&graph, 3, 5, 6);
    addEdge(&graph, 4, 5, 3);
    
    printf("Graph Algorithms Demo\n");
    printf("====================\n\n");
    
    int startNode = 0;
    dijkstra(&graph, startNode);
    printf("\n");
    
    dfs(&graph, startNode);
    
    return 0;
}