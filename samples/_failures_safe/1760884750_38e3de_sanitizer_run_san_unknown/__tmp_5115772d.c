//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 100

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct Graph {
    int numNodes;
    Node* adjacencyList[MAX_NODES];
} Graph;

Graph* createGraph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        return NULL;
    }
    
    Graph* graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        return NULL;
    }
    
    graph->numNodes = nodes;
    for (int i = 0; i < nodes; i++) {
        graph->adjacencyList[i] = NULL;
    }
    
    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    if (graph == NULL || src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        return;
    }
    
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        return;
    }
    newNode->data = dest;
    newNode->next = graph->adjacencyList[src];
    graph->adjacencyList[src] = newNode;
}

void recursiveDFS(Graph* graph, int vertex, int visited[]) {
    if (graph == NULL || vertex < 0 || vertex >= graph->numNodes || visited == NULL) {
        return;
    }
    
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    Node* temp = graph->adjacencyList[vertex];
    while (temp != NULL) {
        if (!visited[temp->data]) {
            recursiveDFS(graph, temp->data, visited);
        }
        temp = temp->next;
    }
}

void performDFS(Graph* graph, int startVertex) {
    if (graph == NULL || startVertex < 0 || startVertex >= graph->numNodes) {
        printf("Invalid input parameters\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from vertex %d: ", startVertex);
    recursiveDFS(graph, startVertex, visited);
    printf("\n");
}

void freeGraph(Graph* graph) {
    if (graph == NULL) {
        return;
    }
    
    for (int i = 0; i < graph->numNodes; i++) {
        Node* current = graph->adjacencyList[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph);
}

int main() {
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &numNodes) != 1 || numNodes < 1 || numNodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    Graph* graph = createGraph(numNodes);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1 || numEdges < 0) {
        printf("Invalid number of edges\n");
        freeGraph(graph);
        return 1;
    }
    
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            printf("Invalid edge input\n");
            freeGraph(graph);
            return 1;
        }
        if (src < 0 || src >= numNodes || dest < 0 || dest >= numNodes) {
            printf("Invalid vertex numbers\n");
            freeGraph(graph);
            return 1;
        }
        addEdge(graph, src, dest);
    }
    
    int startVertex;
    printf("Enter starting vertex for DFS: ");
    if (scanf("%d", &startVertex) != 1 || startVertex < 0 || startVertex >= numNodes) {
        printf("Invalid starting vertex\n");
        freeGraph(graph);
        return 1;
    }
    
    performDFS(graph, startVertex);
    freeGraph(graph);
    
    return 0;
}