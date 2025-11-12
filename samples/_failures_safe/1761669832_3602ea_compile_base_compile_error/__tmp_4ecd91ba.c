//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
c
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
    CYCLE_DETECT
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

int validateVertex(struct Graph *g, int v) {
    return (g != NULL && v >= 0 && v < g->vertices);
}

void bfs(struct Graph *g, int start) {
    if (!validateVertex(g, start)) return;
    
    int *visited = calloc(g->vertices, sizeof(int));
    if (visited == NULL) return;
    
    int *queue = malloc(g->vertices * sizeof(int));
    if (queue == NULL) {
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
        
        if (g->type == ADJ_MATRIX) {
            for (int i = 0; i < g->vertices; i++) {
                if (g->adjMatrix[current][i] && !visited[i]) {
                    visited[i] = 1;
                    queue[rear++] = i;
                }
            }
        } else {
            for (int i = 0; i < g->listSizes[current]; i++) {
                int neighbor = g->adjList[current][i];
                if (!visited[neighbor]) {
                    visited[neighbor] = 1;
                    queue[rear++] = neighbor;
                }
            }
        }
    }
    printf("\n");
    
    free(visited);
    free(queue);
}

void dfsUtil(struct Graph *g, int v, int *visited) {
    visited[v] = 1;
    printf("%d ", v);
    
    if (g->type == ADJ_MATRIX) {
        for (int i = 0; i < g->vertices; i++) {
            if (g->adjMatrix[v][i] && !visited[i]) {
                dfsUtil(g, i, visited);
            }
        }
    } else {
        for (int i = 0; i < g->listSizes[v]; i++) {
            int neighbor = g->adjList[v][i];
            if (!visited[neighbor]) {
                dfsUtil(g, neighbor, visited);
            }
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (!validateVertex(g, start)) return;
    
    int *visited = calloc(g->vertices, sizeof(int));
    if (visited == NULL) return;
    
    printf("DFS from vertex %d: ", start);
    dfsUtil(g, start, visited);
    printf("\n");
    
    free(visited);
}

int detectCycleUtil(struct Graph *g, int v, int *visited, int *recStack) {
    if (!visited[v]) {
        visited[v] = 1;
        recStack[v] = 1;
        
        if (g->type == ADJ_MATRIX) {
            for (int i = 0; i < g->vertices; i++) {
                if (g->adjMatrix[v][i]) {
                    if (!visited[i] && detectCycleUtil(g, i, visited, recStack)) {
                        return 1;
                    } else if (recStack[i]) {
                        return 1;
                    }
                }
            }
        } else {
            for (int i = 0; i < g->listSizes[v]; i++) {
                int neighbor = g->adjList[v][i];
                if (!visited[neighbor] && detectCycleUtil(g, neighbor, visited, recStack)) {
                    return 1;
                } else if (recStack[neighbor]) {
                    return 1;
                }
            }
        }
    }
    recStack[v] = 0;
    return 0;
}

void detectCycle(struct Graph *g) {
    if (g == NULL) return;
    
    int *visited = calloc(g->vertices, sizeof(int));
    int *recStack = calloc(g->vertices, sizeof(int));
    if (visited == NULL || recStack == NULL) {
        free(visited);
        free(recStack);
        return;
    }
    
    int hasCycle = 0;
    for (int i = 0; i < g->vertices; i++) {
        if (!visited[i] && detect