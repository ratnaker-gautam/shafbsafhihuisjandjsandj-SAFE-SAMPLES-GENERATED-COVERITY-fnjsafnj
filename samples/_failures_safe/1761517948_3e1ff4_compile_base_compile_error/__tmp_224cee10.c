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
    struct Node* adjacency_lists[MAX_NODES];
};

struct Node* create_node(int vertex) {
    struct Node* new_node = malloc(sizeof(struct Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->vertex = vertex;
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
        *(graph->adjacency_lists + i) = NULL;
    }
    
    return graph;
}

void add_edge(struct Graph* graph, int src, int dest) {
    if (graph == NULL || src < 0 || src >= graph->num_vertices || 
        dest < 0 || dest >= graph->num_vertices) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(EXIT_FAILURE);
    }
    
    struct Node* new_node = create_node(dest);
    new_node->next = *(graph->adjacency_lists + src);
    *(graph->adjacency_lists + src) = new_node;
    
    new_node = create_node(src);
    new_node->next = *(graph->adjacency_lists + dest);
    *(graph->adjacency_lists + dest) = new_node;
}

void dfs_util(struct Graph* graph, int vertex, int* visited) {
    if (visited == NULL) {
        fprintf(stderr, "Invalid visited array\n");
        exit(EXIT_FAILURE);
    }
    
    *(visited + vertex) = 1;
    printf("%d ", vertex);
    
    struct Node* temp = *(graph->adjacency_lists + vertex);
    while (temp != NULL) {
        int adj_vertex = temp->vertex;
        if (*(visited + adj_vertex) == 0) {
            dfs_util(graph, adj_vertex, visited);
        }
        temp = temp->next;
    }
}

void depth_first_search(struct Graph* graph, int start_vertex) {
    if (graph == NULL || start_vertex < 0 || start_vertex >= graph->num_vertices) {
        fprintf(stderr, "Invalid DFS parameters\n");
        exit(EXIT_FAILURE);
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < graph->num_vertices; i++) {
        *(visited + i) = 0;
    }
    
    printf("DFS traversal starting from vertex %d: ", start_vertex);
    dfs_util(graph, start_vertex, visited);
    printf("\n");
}

void free_graph(struct Graph* graph) {
    if (graph == NULL) return;
    
    for (int i = 0; i < graph->num_vertices; i++) {
        struct Node* current = *(graph->adjacency_lists + i);
        while (current != NULL) {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph);
}

int main() {
    int vertices, edges;
    
    printf("Enter number of vertices (1-%d): ", MAX_NODES);
    if (scanf("%d", &vertices) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (vertices <= 0 || vertices > MAX_NODES) {
        fprintf(stderr, "Invalid number of vertices\n");
        return EXIT_FAILURE;
    }
    
    struct Graph* graph = create_graph(vertices);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        free_graph(graph);
        return EXIT_FAILURE;
    }
    
    if (edges < 0 || edges > vertices * (vertices - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        free_graph(graph);
        return EXIT_FAILURE;
    }
    
    printf("Enter edges (source destination):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest;
        if (scanf("%d %d", &src, &dest) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            free_graph(graph);
            return EXIT_FAILURE;
        }
        
        if (src < 0 || src >= vertices || dest < 0 || dest >= vertices) {
            fprintf(stderr, "Invalid vertex in edge\n");
            free_graph(graph);
            return EXIT_FAILURE;
        }
        
        add_edge(graph, src, dest);
    }
    
    int start_vertex;
    printf("Enter starting vertex for DFS: ");
    if (scanf("%d", &start_vertex) != 1) {
        fprintf(stderr, "Invalid input\n");
        free_graph(graph);