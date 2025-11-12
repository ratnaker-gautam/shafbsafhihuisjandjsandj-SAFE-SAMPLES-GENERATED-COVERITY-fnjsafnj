//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum GraphType { ADJ_MATRIX, ADJ_LIST };
enum Algorithm { BFS, DFS, DIJKSTRA };

struct Graph {
    enum GraphType type;
    int vertices;
    void *data;
};

struct AdjListNode {
    int dest;
    int weight;
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

int addEdge(struct Graph *graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return 0;
    if (weight < 0 || weight > 1000) return 0;
    
    switch (graph->type) {
        case ADJ_MATRIX: {
            int *matrix = (int *)graph->data;
            matrix[src * graph->vertices + dest] = weight;
            matrix[dest * graph->vertices + src] = weight;
            break;
        }
            
        case ADJ_LIST: {
            struct AdjList *adjList = (struct AdjList *)graph->data;
            
            struct AdjListNode *newNode = malloc(sizeof(struct AdjListNode));
            if (!newNode) return 0;
            newNode->dest = dest;
            newNode->weight = weight;
            newNode->next = adjList[src].head;
            adjList[src].head = newNode;
            
            newNode = malloc(sizeof(struct AdjListNode));
            if (!newNode) return 0;
            newNode->dest = src;
            newNode->weight = weight;
            newNode->next = adjList[dest].head;
            adjList[dest].head = newNode;
            break;
        }
    }
    
    return 1;
}

void dfsUtil(struct Graph *graph, int v, int *visited) {
    visited[v] = 1;
    printf("%d ", v);
    
    switch (graph->type) {
        case ADJ_MATRIX: {
            int *matrix = (int *)graph->data;
            for (int i = 0; i < graph->vertices; i++) {
                if (matrix[v * graph->vertices + i] != 0 && !visited[i]) {
                    dfsUtil(graph, i, visited);
                }
            }
            break;
        }
            
        case ADJ_LIST: {
            struct AdjList *adjList = (struct AdjList *)graph->data;
            struct AdjListNode *node = adjList[v].head;
            while (node) {
                if (!visited[node->dest]) {
                    dfsUtil(graph, node->dest, visited);
                }
                node = node->next;
            }
            break;
        }
    }
}

void dfs(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    
    printf("DFS traversal: ");
    dfsUtil(graph, start, visited);
    printf("\n");
    
    free(visited);
}

void bfs(struct Graph *graph, int start) {
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
    
    printf("BFS traversal: ");
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        switch (graph->type) {
            case ADJ_MATRIX: {
                int *matrix = (int *)graph->data;
                for (int i = 0; i < graph->vertices; i++) {
                    if (matrix[current * graph->vertices + i] != 0 && !