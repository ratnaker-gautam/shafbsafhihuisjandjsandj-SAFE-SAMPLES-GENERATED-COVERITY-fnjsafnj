//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum GraphType {
    ADJ_MATRIX,
    ADJ_LIST
};

enum Operation {
    BFS,
    DFS,
    SHORTEST_PATH,
    CYCLE_CHECK
};

struct Graph {
    enum GraphType type;
    int vertices;
    int **adjMatrix;
    int **adjList;
    int *listSizes;
};

void freeGraph(struct Graph *g) {
    if (g == NULL) return;
    
    if (g->type == ADJ_MATRIX && g->adjMatrix != NULL) {
        for (int i = 0; i < g->vertices; i++) {
            if (g->adjMatrix[i] != NULL) {
                free(g->adjMatrix[i]);
            }
        }
        free(g->adjMatrix);
    } else if (g->type == ADJ_LIST && g->adjList != NULL) {
        for (int i = 0; i < g->vertices; i++) {
            if (g->adjList[i] != NULL) {
                free(g->adjList[i]);
            }
        }
        free(g->adjList);
        free(g->listSizes);
    }
}

int initializeGraph(struct Graph *g, enum GraphType type, int vertices) {
    if (g == NULL || vertices <= 0 || vertices > 100) return 0;
    
    g->type = type;
    g->vertices = vertices;
    g->adjMatrix = NULL;
    g->adjList = NULL;
    g->listSizes = NULL;
    
    if (type == ADJ_MATRIX) {
        g->adjMatrix = (int**)malloc(vertices * sizeof(int*));
        if (g->adjMatrix == NULL) return 0;
        
        for (int i = 0; i < vertices; i++) {
            g->adjMatrix[i] = (int*)calloc(vertices, sizeof(int));
            if (g->adjMatrix[i] == NULL) {
                freeGraph(g);
                return 0;
            }
        }
    } else if (type == ADJ_LIST) {
        g->adjList = (int**)malloc(vertices * sizeof(int*));
        g->listSizes = (int*)calloc(vertices, sizeof(int));
        if (g->adjList == NULL || g->listSizes == NULL) {
            freeGraph(g);
            return 0;
        }
        
        for (int i = 0; i < vertices; i++) {
            g->adjList[i] = (int*)malloc(10 * sizeof(int));
            if (g->adjList[i] == NULL) {
                freeGraph(g);
                return 0;
            }
            g->listSizes[i] = 0;
        }
    }
    
    return 1;
}

int addEdge(struct Graph *g, int src, int dest) {
    if (g == NULL || src < 0 || src >= g->vertices || dest < 0 || dest >= g->vertices) return 0;
    
    switch (g->type) {
        case ADJ_MATRIX:
            g->adjMatrix[src][dest] = 1;
            g->adjMatrix[dest][src] = 1;
            break;
        case ADJ_LIST:
            if (g->listSizes[src] < 10) {
                g->adjList[src][g->listSizes[src]++] = dest;
            }
            if (g->listSizes[dest] < 10) {
                g->adjList[dest][g->listSizes[dest]++] = src;
            }
            break;
    }
    return 1;
}

void dfsUtil(struct Graph *g, int v, int *visited) {
    if (g == NULL || visited == NULL || v < 0 || v >= g->vertices) return;
    
    visited[v] = 1;
    printf("%d ", v);
    
    switch (g->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < g->vertices; i++) {
                if (g->adjMatrix[v][i] && !visited[i]) {
                    dfsUtil(g, i, visited);
                }
            }
            break;
        case ADJ_LIST:
            for (int i = 0; i < g->listSizes[v]; i++) {
                int neighbor = g->adjList[v][i];
                if (neighbor >= 0 && neighbor < g->vertices && !visited[neighbor]) {
                    dfsUtil(g, neighbor, visited);
                }
            }
            break;
    }
}

void dfs(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->vertices) return;
    
    int *visited = (int*)calloc(g->vertices, sizeof(int));
    if (visited == NULL) return;
    
    printf("DFS traversal: ");
    dfsUtil(g, start, visited);
    printf("\n");
    
    free(visited);
}

void bfs(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->vertices) return;
    
    int *visited = (int*)calloc(g->vertices, sizeof(int));
    if (visited == NULL) return;
    
    int *queue = (int*)malloc(g->vertices * sizeof(int));
    if (queue == NULL) {
        free(visited);
        return;
    }
    
    int front = 0, rear =