//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum GraphType { ADJ_MATRIX, ADJ_LIST };

struct Graph {
    enum GraphType type;
    int vertices;
    union {
        int **matrix;
        struct Node **list;
    } data;
};

struct Node {
    int vertex;
    int weight;
    struct Node *next;
};

struct Graph* create_graph(enum GraphType type, int vertices) {
    if (vertices <= 0 || vertices > 100) return NULL;
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->type = type;
    graph->vertices = vertices;
    
    if (type == ADJ_MATRIX) {
        graph->data.matrix = malloc(vertices * sizeof(int*));
        if (!graph->data.matrix) {
            free(graph);
            return NULL;
        }
        
        for (int i = 0; i < vertices; i++) {
            graph->data.matrix[i] = calloc(vertices, sizeof(int));
            if (!graph->data.matrix[i]) {
                for (int j = 0; j < i; j++) free(graph->data.matrix[j]);
                free(graph->data.matrix);
                free(graph);
                return NULL;
            }
        }
    } else {
        graph->data.list = calloc(vertices, sizeof(struct Node*));
        if (!graph->data.list) {
            free(graph);
            return NULL;
        }
    }
    
    return graph;
}

void add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return;
    
    switch (graph->type) {
        case ADJ_MATRIX:
            if (weight < 0 || weight > 1000) return;
            graph->data.matrix[src][dest] = weight;
            graph->data.matrix[dest][src] = weight;
            break;
            
        case ADJ_LIST:
            if (weight < 0 || weight > 1000) return;
            struct Node *new_node = malloc(sizeof(struct Node));
            if (!new_node) return;
            new_node->vertex = dest;
            new_node->weight = weight;
            new_node->next = graph->data.list[src];
            graph->data.list[src] = new_node;
            
            new_node = malloc(sizeof(struct Node));
            if (!new_node) return;
            new_node->vertex = src;
            new_node->weight = weight;
            new_node->next = graph->data.list[dest];
            graph->data.list[dest] = new_node;
            break;
    }
}

void dfs_util(struct Graph *graph, int vertex, int *visited) {
    if (!graph || !visited || vertex < 0 || vertex >= graph->vertices) return;
    
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    switch (graph->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < graph->vertices; i++) {
                if (graph->data.matrix[vertex][i] != 0 && !visited[i]) {
                    dfs_util(graph, i, visited);
                }
            }
            break;
            
        case ADJ_LIST:
            struct Node *temp = graph->data.list[vertex];
            while (temp) {
                if (!visited[temp->vertex]) {
                    dfs_util(graph, temp->vertex, visited);
                }
                temp = temp->next;
            }
            break;
    }
}

void dfs(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    
    printf("DFS traversal: ");
    dfs_util(graph, start, visited);
    printf("\n");
    free(visited);
}

void free_graph(struct Graph *graph) {
    if (!graph) return;
    
    switch (graph->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < graph->vertices; i++) {
                free(graph->data.matrix[i]);
            }
            free(graph->data.matrix);
            break;
            
        case ADJ_LIST:
            for (int i = 0; i < graph->vertices; i++) {
                struct Node *temp = graph->data.list[i];
                while (temp) {
                    struct Node *to_free = temp;
                    temp = temp->next;
                    free(to_free);
                }
            }
            free(graph->data.list);
            break;
    }
    free(graph);
}

int main() {
    printf("Graph Algorithms Demo\n");
    
    struct Graph *matrix_graph = create_graph(ADJ_MATRIX, 5);
    if (!matrix_graph) {
        printf("Failed to create matrix graph\n");
        return 1;
    }
    
    add_edge(matrix_graph, 0, 1, 2);
    add_edge(matrix_graph, 0, 2, 3);
    add_edge(matrix_graph, 1, 3, 1);
    add_edge(matrix_graph, 2, 4, 4);
    
    dfs(matrix_graph, 0);
    
    struct Graph *list_graph = create_graph(ADJ_LIST, 4);
    if (!list_graph) {
        printf("Failed to create list graph\n");
        free_graph(matrix_graph);