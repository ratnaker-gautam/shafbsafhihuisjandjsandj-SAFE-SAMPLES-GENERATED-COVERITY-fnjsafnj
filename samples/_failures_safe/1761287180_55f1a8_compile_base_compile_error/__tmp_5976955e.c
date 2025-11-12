//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum GraphType { UNDIRECTED, DIRECTED };
enum Operation { ADD_EDGE, REMOVE_EDGE, FIND_PATH, PRINT_GRAPH, EXIT };

struct Graph {
    int vertices;
    int **adjMatrix;
    enum GraphType type;
};

struct Graph* createGraph(int vertices, enum GraphType type) {
    if (vertices <= 0 || vertices > 100) return NULL;
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->vertices = vertices;
    graph->type = type;
    graph->adjMatrix = malloc(vertices * sizeof(int*));
    if (!graph->adjMatrix) { free(graph); return NULL; }
    for (int i = 0; i < vertices; i++) {
        graph->adjMatrix[i] = calloc(vertices, sizeof(int));
        if (!graph->adjMatrix[i]) {
            for (int j = 0; j < i; j++) free(graph->adjMatrix[j]);
            free(graph->adjMatrix);
            free(graph);
            return NULL;
        }
    }
    return graph;
}

void freeGraph(struct Graph *graph) {
    if (!graph) return;
    for (int i = 0; i < graph->vertices; i++) free(graph->adjMatrix[i]);
    free(graph->adjMatrix);
    free(graph);
}

int addEdge(struct Graph *graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return 0;
    graph->adjMatrix[src][dest] = 1;
    if (graph->type == UNDIRECTED) graph->adjMatrix[dest][src] = 1;
    return 1;
}

int removeEdge(struct Graph *graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return 0;
    graph->adjMatrix[src][dest] = 0;
    if (graph->type == UNDIRECTED) graph->adjMatrix[dest][src] = 0;
    return 1;
}

int findPathDFS(struct Graph *graph, int src, int dest, int *visited) {
    if (src == dest) return 1;
    visited[src] = 1;
    for (int i = 0; i < graph->vertices; i++) {
        if (graph->adjMatrix[src][i] && !visited[i] && findPathDFS(graph, i, dest, visited)) return 1;
    }
    return 0;
}

int findPath(struct Graph *graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return 0;
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return 0;
    int result = findPathDFS(graph, src, dest, visited);
    free(visited);
    return result;
}

void printGraph(struct Graph *graph) {
    if (!graph) return;
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < graph->vertices; i++) {
        for (int j = 0; j < graph->vertices; j++) printf("%d ", graph->adjMatrix[i][j]);
        printf("\n");
    }
}

int main() {
    struct Graph *graph = NULL;
    int vertices, choice, src, dest, typeChoice;
    printf("Enter number of vertices (1-100): ");
    if (scanf("%d", &vertices) != 1 || vertices < 1 || vertices > 100) {
        printf("Invalid vertices\n");
        return 1;
    }
    printf("Graph type: 0=Undirected, 1=Directed: ");
    if (scanf("%d", &typeChoice) != 1 || (typeChoice != 0 && typeChoice != 1)) {
        printf("Invalid type\n");
        return 1;
    }
    graph = createGraph(vertices, typeChoice ? DIRECTED : UNDIRECTED);
    if (!graph) {
        printf("Graph creation failed\n");
        return 1;
    }
    while (1) {
        printf("\n0=Add Edge, 1=Remove Edge, 2=Find Path, 3=Print Graph, 4=Exit\nChoice: ");
        if (scanf("%d", &choice) != 1 || choice < 0 || choice > 4) {
            printf("Invalid choice\n");
            continue;
        }
        enum Operation op = choice;
        switch (op) {
            case ADD_EDGE:
                printf("Enter source and destination: ");
                if (scanf("%d %d", &src, &dest) != 2 || src < 0 || src >= vertices || dest < 0 || dest >= vertices) {
                    printf("Invalid vertices\n");
                    break;
                }
                if (addEdge(graph, src, dest)) printf("Edge added\n");
                else printf("Failed to add edge\n");
                break;
            case REMOVE_EDGE:
                printf("Enter source and destination: ");
                if (scanf("%d %d", &src, &dest) != 2 || src < 0 || src >= vertices || dest < 0 || dest >= vertices) {
                    printf("Invalid vertices\n");
                    break;
                }
                if (removeEdge(graph, src,