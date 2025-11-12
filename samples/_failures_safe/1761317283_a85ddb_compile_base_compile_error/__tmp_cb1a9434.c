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
    int num_vertices;
    struct Node* adj_lists[MAX_NODES];
    int visited[MAX_NODES];
};

struct Node* create_node(int v) {
    struct Node* new_node = malloc(sizeof(struct Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->vertex = v;
    new_node->next = NULL;
    return new_node;
}

struct Graph* create_graph(int vertices) {
    if (vertices <= 0 || vertices > MAX_NODES) {
        fprintf(stderr, "Invalid number of vertices\n");
        exit(EXIT_FAILURE);
    }
    
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    graph->num_vertices = vertices;
    
    for (int i = 0; i < vertices; i++) {
        *(graph->adj_lists + i) = NULL;
        *(graph->visited + i) = 0;
    }
    
    return graph;
}

void add_edge(struct Graph* graph, int src, int dest) {
    if (graph == NULL) return;
    if (src < 0 || src >= graph->num_vertices || dest < 0 || dest >= graph->num_vertices) {
        fprintf(stderr, "Invalid vertex index\n");
        return;
    }
    
    struct Node* new_node = create_node(dest);
    new_node->next = *(graph->adj_lists + src);
    *(graph->adj_lists + src) = new_node;
    
    new_node = create_node(src);
    new_node->next = *(graph->adj_lists + dest);
    *(graph->adj_lists + dest) = new_node;
}

void dfs_util(struct Graph* graph, int vertex, int* result, int* index) {
    if (graph == NULL || result == NULL || index == NULL) return;
    if (vertex < 0 || vertex >= graph->num_vertices) return;
    
    *(graph->visited + vertex) = 1;
    *(result + *index) = vertex;
    (*index)++;
    
    struct Node* temp = *(graph->adj_lists + vertex);
    while (temp != NULL) {
        int adj_vertex = temp->vertex;
        if (*(graph->visited + adj_vertex) == 0) {
            dfs_util(graph, adj_vertex, result, index);
        }
        temp = temp->next;
    }
}

void dfs(struct Graph* graph, int start_vertex, int* result, int* result_size) {
    if (graph == NULL || result == NULL || result_size == NULL) return;
    if (start_vertex < 0 || start_vertex >= graph->num_vertices) return;
    
    for (int i = 0; i < graph->num_vertices; i++) {
        *(graph->visited + i) = 0;
    }
    
    *result_size = 0;
    dfs_util(graph, start_vertex, result, result_size);
}

void bfs(struct Graph* graph, int start_vertex, int* result, int* result_size) {
    if (graph == NULL || result == NULL || result_size == NULL) return;
    if (start_vertex < 0 || start_vertex >= graph->num_vertices) return;
    
    for (int i = 0; i < graph->num_vertices; i++) {
        *(graph->visited + i) = 0;
    }
    
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    *(graph->visited + start_vertex) = 1;
    *(queue + rear) = start_vertex;
    rear++;
    
    *result_size = 0;
    
    while (front < rear) {
        int current_vertex = *(queue + front);
        front++;
        
        *(result + *result_size) = current_vertex;
        (*result_size)++;
        
        struct Node* temp = *(graph->adj_lists + current_vertex);
        while (temp != NULL) {
            int adj_vertex = temp->vertex;
            if (*(graph->visited + adj_vertex) == 0) {
                *(graph->visited + adj_vertex) = 1;
                *(queue + rear) = adj_vertex;
                rear++;
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
            }
            temp = temp->next;
        }
    }
}

void free_graph(struct Graph* graph) {
    if (graph == NULL) return;
    
    for (int i = 0; i < graph->num_vertices; i++) {
        struct Node* temp = *(graph->adj_lists + i);
        while (temp != NULL) {
            struct Node* to_free = temp;
            temp = temp->next;
            free(to_free);
        }
    }
    free(graph);
}

int main() {
    struct Graph* graph = create_graph(6);
    
    add_edge(graph, 0, 1);
    add_edge(graph, 0, 2);
    add_edge(graph, 1, 3);
    add_edge(graph, 2, 3);