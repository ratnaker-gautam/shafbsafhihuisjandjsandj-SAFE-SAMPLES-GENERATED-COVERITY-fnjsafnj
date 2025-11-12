//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum GraphType { ADJ_MATRIX, ADJ_LIST };

struct Graph {
    enum GraphType type;
    int vertices;
    void *data;
};

struct AdjListNode {
    int dest;
    struct AdjListNode *next;
};

struct AdjList {
    struct AdjListNode *head;
};

void freeGraph(struct Graph *graph) {
    if (!graph) return;
    
    if (graph->type == ADJ_LIST && graph->data) {
        struct AdjList *adjList = (struct AdjList *)graph->data;
        for (int i = 0; i < graph->vertices; i++) {
            struct AdjListNode *current = adjList[i].head;
            while (current) {
                struct AdjListNode *temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(adjList);
    } else if (graph->type == ADJ_MATRIX && graph->data) {
        free(graph->data);
    }
    free(graph);
}

struct Graph *createGraph(enum GraphType type, int vertices) {
    if (vertices <= 0 || vertices > 100) return NULL;
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->type = type;
    graph->vertices = vertices;
    
    switch (type) {
        case ADJ_MATRIX:
            graph->data = calloc(vertices * vertices, sizeof(int));
            if (!graph->data) {
                free(graph);
                return NULL;
            }
            break;
            
        case ADJ_LIST:
            graph->data = calloc(vertices, sizeof(struct AdjList));
            if (!graph->data) {
                free(graph);
                return NULL;
            }
            break;
    }
    
    return graph;
}

int addEdge(struct Graph *graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return 0;
    
    switch (graph->type) {
        case ADJ_MATRIX:
            {
                int *matrix = (int *)graph->data;
                matrix[src * graph->vertices + dest] = 1;
                matrix[dest * graph->vertices + src] = 1;
            }
            break;
            
        case ADJ_LIST:
            {
                struct AdjList *adjList = (struct AdjList *)graph->data;
                
                struct AdjListNode *newNode = malloc(sizeof(struct AdjListNode));
                if (!newNode) return 0;
                newNode->dest = dest;
                newNode->next = adjList[src].head;
                adjList[src].head = newNode;
                
                newNode = malloc(sizeof(struct AdjListNode));
                if (!newNode) return 0;
                newNode->dest = src;
                newNode->next = adjList[dest].head;
                adjList[dest].head = newNode;
            }
            break;
    }
    
    return 1;
}

void printGraph(struct Graph *graph) {
    if (!graph) return;
    
    switch (graph->type) {
        case ADJ_MATRIX:
            {
                printf("Adjacency Matrix:\n");
                int *matrix = (int *)graph->data;
                for (int i = 0; i < graph->vertices; i++) {
                    for (int j = 0; j < graph->vertices; j++) {
                        printf("%d ", matrix[i * graph->vertices + j]);
                    }
                    printf("\n");
                }
            }
            break;
            
        case ADJ_LIST:
            {
                printf("Adjacency List:\n");
                struct AdjList *adjList = (struct AdjList *)graph->data;
                for (int i = 0; i < graph->vertices; i++) {
                    printf("%d: ", i);
                    struct AdjListNode *current = adjList[i].head;
                    while (current) {
                        printf("%d ", current->dest);
                        current = current->next;
                    }
                    printf("\n");
                }
            }
            break;
    }
}

void BFS(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    
    int *queue = malloc(graph->vertices * sizeof(int));
    if (!queue) {
        free(visited);
        return;
    }
    
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS from vertex %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        switch (graph->type) {
            case ADJ_MATRIX:
                {
                    int *matrix = (int *)graph->data;
                    for (int i = 0; i < graph->vertices; i++) {
                        if (matrix[current * graph->vertices + i] && !visited[i]) {
                            visited[i] = 1;
                            queue[rear++] = i;
                        }
                    }
                }
                break;
                
            case ADJ_LIST:
                {
                    struct AdjList *adjList = (struct AdjList *)graph->data;
                    struct AdjListNode *node = adjList[current].head;
                    while (node) {
                        if (!visited[node->dest]) {
                            visited[node->dest] = 1;
                            queue[rear++]