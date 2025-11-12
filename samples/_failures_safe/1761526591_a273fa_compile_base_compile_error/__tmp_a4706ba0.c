//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum GraphType { ADJ_MATRIX, ADJ_LIST };

struct Graph {
    enum GraphType type;
    int vertices;
    union {
        int **matrix;
        struct Node **list;
    } representation;
};

struct Node {
    int vertex;
    int weight;
    struct Node *next;
};

void freeGraph(struct Graph *graph) {
    if (graph == NULL) return;
    
    if (graph->type == ADJ_MATRIX) {
        if (graph->representation.matrix != NULL) {
            for (int i = 0; i < graph->vertices; i++) {
                if (graph->representation.matrix[i] != NULL) {
                    free(graph->representation.matrix[i]);
                }
            }
            free(graph->representation.matrix);
        }
    } else {
        if (graph->representation.list != NULL) {
            for (int i = 0; i < graph->vertices; i++) {
                struct Node *current = graph->representation.list[i];
                while (current != NULL) {
                    struct Node *temp = current;
                    current = current->next;
                    free(temp);
                }
            }
            free(graph->representation.list);
        }
    }
    free(graph);
}

int validateVertex(struct Graph *graph, int vertex) {
    return graph != NULL && vertex >= 0 && vertex < graph->vertices;
}

void bfs(struct Graph *graph, int start) {
    if (!validateVertex(graph, start)) return;
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (visited == NULL) return;
    
    int *queue = malloc(graph->vertices * sizeof(int));
    if (queue == NULL) {
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
            case ADJ_MATRIX:
                for (int i = 0; i < graph->vertices; i++) {
                    if (graph->representation.matrix[current][i] != 0 && !visited[i]) {
                        visited[i] = 1;
                        queue[rear++] = i;
                    }
                }
                break;
                
            case ADJ_LIST:
                {
                    struct Node *temp = graph->representation.list[current];
                    while (temp != NULL) {
                        if (!visited[temp->vertex]) {
                            visited[temp->vertex] = 1;
                            queue[rear++] = temp->vertex;
                        }
                        temp = temp->next;
                    }
                }
                break;
        }
    }
    printf("\n");
    
    free(visited);
    free(queue);
}

void dfsUtil(struct Graph *graph, int vertex, int *visited) {
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    switch (graph->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < graph->vertices; i++) {
                if (graph->representation.matrix[vertex][i] != 0 && !visited[i]) {
                    dfsUtil(graph, i, visited);
                }
            }
            break;
            
        case ADJ_LIST:
            {
                struct Node *temp = graph->representation.list[vertex];
                while (temp != NULL) {
                    if (!visited[temp->vertex]) {
                        dfsUtil(graph, temp->vertex, visited);
                    }
                    temp = temp->next;
                }
            }
            break;
    }
}

void dfs(struct Graph *graph, int start) {
    if (!validateVertex(graph, start)) return;
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (visited == NULL) return;
    
    printf("DFS traversal: ");
    dfsUtil(graph, start, visited);
    printf("\n");
    
    free(visited);
}

struct Graph* createMatrixGraph(int vertices) {
    if (vertices <= 0 || vertices > 100) return NULL;
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (graph == NULL) return NULL;
    
    graph->type = ADJ_MATRIX;
    graph->vertices = vertices;
    
    graph->representation.matrix = malloc(vertices * sizeof(int *));
    if (graph->representation.matrix == NULL) {
        free(graph);
        return NULL;
    }
    
    for (int i = 0; i < vertices; i++) {
        graph->representation.matrix[i] = calloc(vertices, sizeof(int));
        if (graph->representation.matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(graph->representation.matrix[j]);
            }
            free(graph->representation.matrix);
            free(graph);
            return NULL;
        }
    }
    
    return graph;
}

void addMatrixEdge(struct Graph *graph, int src, int dest, int weight) {
    if (graph == NULL || graph->type != ADJ_MATRIX) return;
    if (!validateVertex(graph, src) || !validateVertex(graph, dest)) return;
    if (weight < 0 || weight > 1000) return;
    
    graph->representation.matrix[src][dest] = weight;
    graph->representation.matrix[dest][src] = weight;
}

int main() {
    printf("Graph Algorithms Demo\n");
    
    struct Graph *graph