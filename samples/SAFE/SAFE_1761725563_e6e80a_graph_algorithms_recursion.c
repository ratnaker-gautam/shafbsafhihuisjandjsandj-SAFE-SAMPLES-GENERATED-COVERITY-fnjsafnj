//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    int numVertices;
    Node** adjLists;
    int* visited;
} Graph;

Node* createNode(int v) {
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) exit(1);
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int vertices) {
    if (vertices <= 0 || vertices > MAX_NODES) return NULL;
    Graph* graph = malloc(sizeof(Graph));
    if (graph == NULL) return NULL;
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(Node*));
    graph->visited = malloc(vertices * sizeof(int));
    if (graph->adjLists == NULL || graph->visited == NULL) {
        free(graph->adjLists);
        free(graph->visited);
        free(graph);
        return NULL;
    }
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    if (graph == NULL) return;
    if (src < 0 || src >= graph->numVertices) return;
    if (dest < 0 || dest >= graph->numVertices) return;
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void recursiveDFS(Graph* graph, int vertex) {
    if (graph == NULL) return;
    if (vertex < 0 || vertex >= graph->numVertices) return;
    graph->visited[vertex] = 1;
    printf("%d ", vertex);
    Node* temp = graph->adjLists[vertex];
    while (temp != NULL) {
        int adjVertex = temp->vertex;
        if (graph->visited[adjVertex] == 0) {
            recursiveDFS(graph, adjVertex);
        }
        temp = temp->next;
    }
}

void resetVisited(Graph* graph) {
    if (graph == NULL) return;
    for (int i = 0; i < graph->numVertices; i++) {
        graph->visited[i] = 0;
    }
}

void freeGraph(Graph* graph) {
    if (graph == NULL) return;
    for (int i = 0; i < graph->numVertices; i++) {
        Node* temp = graph->adjLists[i];
        while (temp != NULL) {
            Node* toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }
    free(graph->adjLists);
    free(graph->visited);
    free(graph);
}

int main() {
    int vertices, edges;
    printf("Enter number of vertices (1-100): ");
    if (scanf("%d", &vertices) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (vertices < 1 || vertices > MAX_NODES) {
        printf("Invalid number of vertices\n");
        return 1;
    }
    Graph* graph = createGraph(vertices);
    if (graph == NULL) {
        printf("Graph creation failed\n");
        return 1;
    }
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input\n");
        freeGraph(graph);
        return 1;
    }
    if (edges < 0 || edges > vertices * (vertices - 1) / 2) {
        printf("Invalid number of edges\n");
        freeGraph(graph);
        return 1;
    }
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            printf("Invalid edge input\n");
            freeGraph(graph);
            return 1;
        }
        if (src < 0 || src >= vertices || dest < 0 || dest >= vertices) {
            printf("Invalid vertex in edge\n");
            freeGraph(graph);
            return 1;
        }
        addEdge(graph, src, dest);
    }
    printf("DFS traversal starting from vertex 0: ");
    recursiveDFS(graph, 0);
    printf("\n");
    resetVisited(graph);
    printf("DFS traversal starting from vertex %d: ", vertices / 2);
    recursiveDFS(graph, vertices / 2);
    printf("\n");
    freeGraph(graph);
    return 0;
}