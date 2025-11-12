//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct Graph {
    int numNodes;
    Node* adjacencyList[MAX_NODES];
} Graph;

Graph* createGraph(int numNodes) {
    if (numNodes <= 0 || numNodes > MAX_NODES) {
        return NULL;
    }
    Graph* graph = malloc(sizeof(Graph));
    if (!graph) {
        return NULL;
    }
    graph->numNodes = numNodes;
    for (int i = 0; i < numNodes; i++) {
        graph->adjacencyList[i] = NULL;
    }
    return graph;
}

int addEdge(Graph* graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        return 0;
    }
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) {
        return 0;
    }
    newNode->data = dest;
    newNode->next = graph->adjacencyList[src];
    graph->adjacencyList[src] = newNode;
    return 1;
}

void recursiveDFS(Graph* graph, int node, int* visited) {
    if (!graph || !visited || node < 0 || node >= graph->numNodes) {
        return;
    }
    if (visited[node]) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    Node* current = graph->adjacencyList[node];
    while (current) {
        recursiveDFS(graph, current->data, visited);
        current = current->next;
    }
}

void performDFS(Graph* graph, int startNode) {
    if (!graph || startNode < 0 || startNode >= graph->numNodes) {
        printf("Invalid graph or start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from node %d: ", startNode);
    recursiveDFS(graph, startNode, visited);
    printf("\n");
}

void freeGraph(Graph* graph) {
    if (!graph) {
        return;
    }
    for (int i = 0; i < graph->numNodes; i++) {
        Node* current = graph->adjacencyList[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph);
}

int main() {
    int numNodes = 6;
    Graph* graph = createGraph(numNodes);
    if (!graph) {
        printf("Failed to create graph\n");
        return 1;
    }
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 4);
    addEdge(graph, 3, 5);
    addEdge(graph, 4, 5);
    performDFS(graph, 0);
    freeGraph(graph);
    return 0;
}