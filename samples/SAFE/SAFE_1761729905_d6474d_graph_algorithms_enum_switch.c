//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum GraphType { UNDIRECTED, DIRECTED };
enum Algorithm { BFS, DFS, DIJKSTRA, TOPOLOGICAL };

struct Graph {
    int vertices;
    int **adjMatrix;
    enum GraphType type;
};

struct Graph* createGraph(int vertices, enum GraphType type) {
    if (vertices <= 0 || vertices > 100) {
        return NULL;
    }
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) {
        return NULL;
    }
    
    graph->vertices = vertices;
    graph->type = type;
    
    graph->adjMatrix = malloc(vertices * sizeof(int*));
    if (!graph->adjMatrix) {
        free(graph);
        return NULL;
    }
    
    for (int i = 0; i < vertices; i++) {
        graph->adjMatrix[i] = calloc(vertices, sizeof(int));
        if (!graph->adjMatrix[i]) {
            for (int j = 0; j < i; j++) {
                free(graph->adjMatrix[j]);
            }
            free(graph->adjMatrix);
            free(graph);
            return NULL;
        }
    }
    
    return graph;
}

void addEdge(struct Graph *graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) {
        return;
    }
    
    if (weight < 0 || weight > 1000) {
        return;
    }
    
    graph->adjMatrix[src][dest] = weight;
    if (graph->type == UNDIRECTED) {
        graph->adjMatrix[dest][src] = weight;
    }
}

void bfs(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) {
        return;
    }
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) {
        return;
    }
    
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
        
        for (int i = 0; i < graph->vertices; i++) {
            if (graph->adjMatrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < graph->vertices) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
    
    free(visited);
    free(queue);
}

void dfsUtil(struct Graph *graph, int vertex, int *visited) {
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    for (int i = 0; i < graph->vertices; i++) {
        if (graph->adjMatrix[vertex][i] != 0 && !visited[i]) {
            dfsUtil(graph, i, visited);
        }
    }
}

void dfs(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) {
        return;
    }
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) {
        return;
    }
    
    printf("DFS traversal: ");
    dfsUtil(graph, start, visited);
    printf("\n");
    
    free(visited);
}

void freeGraph(struct Graph *graph) {
    if (!graph) {
        return;
    }
    
    for (int i = 0; i < graph->vertices; i++) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    free(graph);
}

int main() {
    struct Graph *graph = createGraph(6, UNDIRECTED);
    if (!graph) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    addEdge(graph, 0, 1, 1);
    addEdge(graph, 0, 2, 1);
    addEdge(graph, 1, 3, 1);
    addEdge(graph, 1, 4, 1);
    addEdge(graph, 2, 4, 1);
    addEdge(graph, 3, 4, 1);
    addEdge(graph, 3, 5, 1);
    addEdge(graph, 4, 5, 1);
    
    printf("Graph with 6 vertices created\n");
    
    enum Algorithm algo = BFS;
    
    switch (algo) {
        case BFS:
            bfs(graph, 0);
            break;
        case DFS:
            dfs(graph, 0);
            break;
        case DIJKSTRA:
            printf("Dijkstra algorithm selected (not implemented)\n");
            break;
        case TOPOLOGICAL:
            printf("Topological sort selected (not implemented)\n");
            break;
        default:
            printf("Unknown algorithm\n");
            break;
    }
    
    freeGraph(graph);
    return 0;
}