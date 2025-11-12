//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

struct Node {
    int vertex;
    struct Node* next;
};

struct Graph {
    int numVertices;
    struct Node** adjLists;
    int* visited;
};

struct Node* createNode(int v) {
    struct Node* newNode = malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int vertices) {
    if (vertices <= 0 || vertices > MAX_NODES) {
        fprintf(stderr, "Invalid number of vertices\n");
        exit(EXIT_FAILURE);
    }
    
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    graph->numVertices = vertices;
    graph->adjLists = malloc((size_t)vertices * sizeof(struct Node*));
    graph->visited = malloc((size_t)vertices * sizeof(int));
    
    if (graph->adjLists == NULL || graph->visited == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < vertices; i++) {
        *(graph->adjLists + i) = NULL;
        *(graph->visited + i) = 0;
    }
    
    return graph;
}

void addEdge(struct Graph* graph, int src, int dest) {
    if (graph == NULL) return;
    if (src < 0 || src >= graph->numVertices || dest < 0 || dest >= graph->numVertices) {
        fprintf(stderr, "Invalid vertex index\n");
        return;
    }
    
    struct Node* newNode = createNode(dest);
    newNode->next = *(graph->adjLists + src);
    *(graph->adjLists + src) = newNode;
    
    newNode = createNode(src);
    newNode->next = *(graph->adjLists + dest);
    *(graph->adjLists + dest) = newNode;
}

void DFS(struct Graph* graph, int vertex) {
    if (graph == NULL) return;
    if (vertex < 0 || vertex >= graph->numVertices) return;
    
    *(graph->visited + vertex) = 1;
    printf("Visited %d\n", vertex);
    
    struct Node* temp = *(graph->adjLists + vertex);
    while (temp != NULL) {
        int connectedVertex = temp->vertex;
        if (*(graph->visited + connectedVertex) == 0) {
            DFS(graph, connectedVertex);
        }
        temp = temp->next;
    }
}

void BFS(struct Graph* graph, int startVertex) {
    if (graph == NULL) return;
    if (startVertex < 0 || startVertex >= graph->numVertices) return;
    
    for (int i = 0; i < graph->numVertices; i++) {
        *(graph->visited + i) = 0;
    }
    
    int queue[MAX_NODES];
    int front = 0;
    int rear = 0;
    
    *(graph->visited + startVertex) = 1;
    *(queue + rear) = startVertex;
    rear++;
    
    while (front < rear) {
        int currentVertex = *(queue + front);
        printf("Visited %d\n", currentVertex);
        front++;
        
        struct Node* temp = *(graph->adjLists + currentVertex);
        while (temp != NULL) {
            int adjVertex = temp->vertex;
            if (*(graph->visited + adjVertex) == 0) {
                *(graph->visited + adjVertex) = 1;
                if (rear < MAX_NODES) {
                    *(queue + rear) = adjVertex;
                    rear++;
                }
            }
            temp = temp->next;
        }
    }
}

void freeGraph(struct Graph* graph) {
    if (graph == NULL) return;
    
    for (int i = 0; i < graph->numVertices; i++) {
        struct Node* temp = *(graph->adjLists + i);
        while (temp != NULL) {
            struct Node* toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }
    
    free(graph->adjLists);
    free(graph->visited);
    free(graph);
}

int main() {
    int vertices, edges;
    
    printf("Enter number of vertices (1-100): ");
    if (scanf("%d", &vertices) != 1 || vertices < 1 || vertices > MAX_NODES) {
        fprintf(stderr, "Invalid input for vertices\n");
        return EXIT_FAILURE;
    }
    
    struct Graph* graph = createGraph(vertices);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        fprintf(stderr, "Invalid input for edges\n");
        freeGraph(graph);
        return EXIT_FAILURE;
    }
    
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            freeGraph