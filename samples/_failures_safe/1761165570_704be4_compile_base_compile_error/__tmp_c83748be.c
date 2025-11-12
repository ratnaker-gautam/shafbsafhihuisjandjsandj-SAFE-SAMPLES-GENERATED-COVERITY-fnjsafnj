//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 20

struct Graph {
    int adjMatrix[MAX_NODES][MAX_NODES];
    int numNodes;
};

void initializeGraph(struct Graph* graph, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    
    graph->numNodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(1);
    }
    
    graph->adjMatrix[src][dest] = 1;
    graph->adjMatrix[dest][src] = 1;
}

void dfsRecursive(struct Graph* graph, int node, int visited[]) {
    if (node < 0 || node >= graph->numNodes) {
        return;
    }
    
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[node][i] && !visited[i]) {
            dfsRecursive(graph, i, visited);
        }
    }
}

void depthFirstSearch(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->numNodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from node %d: ", startNode);
    dfsRecursive(graph, startNode, visited);
    printf("\n");
}

int findPathRecursive(struct Graph* graph, int current, int target, int visited[], int path[], int pathIndex) {
    if (current < 0 || current >= graph->numNodes || target < 0 || target >= graph->numNodes) {
        return 0;
    }
    
    visited[current] = 1;
    path[pathIndex] = current;
    pathIndex++;
    
    if (current == target) {
        printf("Path found: ");
        for (int i = 0; i < pathIndex; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
        return 1;
    }
    
    for (int i = 0; i < graph->numNodes; i++) {
        if (graph->adjMatrix[current][i] && !visited[i]) {
            if (findPathRecursive(graph, i, target, visited, path, pathIndex)) {
                return 1;
            }
        }
    }
    
    return 0;
}

void findPath(struct Graph* graph, int start, int end) {
    if (start < 0 || start >= graph->numNodes || end < 0 || end >= graph->numNodes) {
        fprintf(stderr, "Invalid path parameters\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int path[MAX_NODES];
    
    if (!findPathRecursive(graph, start, end, visited, path, 0)) {
        printf("No path exists between nodes %d and %d\n", start, end);
    }
}

int main() {
    struct Graph graph;
    int choice, nodes, edges, src, dest, start, end;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid input for nodes\n");
        return 1;
    }
    
    initializeGraph(&graph, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        fprintf(stderr, "Invalid input for edges\n");
        return 1;
    }
    
    for (int i = 0; i < edges; i++) {
        printf("Enter edge %d (source destination): ", i + 1);
        if (scanf("%d %d", &src, &dest) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        addEdge(&graph, src, dest);
    }
    
    while (1) {
        printf("\nGraph Operations:\n");
        printf("1. Depth First Search\n");
        printf("2. Find Path Between Nodes\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid choice input\n");
            return 1;
        }
        
        switch (choice) {
            case 1:
                printf("Enter start node for DFS: ");
                if (scanf("%d", &start) != 1) {
                    fprintf(stderr, "Invalid start node\n");
                    return 1;
                }
                depthFirstSearch(&graph, start);
                break;
                
            case 2:
                printf("Enter start and end nodes: ");
                if (scanf("%d %d", &start, &end) != 2)