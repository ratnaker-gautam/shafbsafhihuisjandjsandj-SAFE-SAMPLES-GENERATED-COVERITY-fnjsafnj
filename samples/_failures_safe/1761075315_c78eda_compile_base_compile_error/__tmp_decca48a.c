//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define MAX_EDGES 1000

struct Graph {
    int vertices;
    int edges;
    int adjacency[MAX_VERTICES][MAX_VERTICES];
};

struct Edge {
    int src;
    int dest;
    int weight;
};

void initialize_graph(struct Graph* graph, int vertices) {
    if (vertices <= 0 || vertices > MAX_VERTICES) {
        vertices = MAX_VERTICES;
    }
    graph->vertices = vertices;
    graph->edges = 0;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            graph->adjacency[i][j] = 0;
        }
    }
}

int add_edge(struct Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) {
        return 0;
    }
    if (graph->edges >= MAX_EDGES) {
        return 0;
    }
    if (src == dest) {
        return 0;
    }
    graph->adjacency[src][dest] = weight;
    graph->adjacency[dest][src] = weight;
    graph->edges++;
    return 1;
}

void print_graph(struct Graph* graph) {
    printf("Graph with %d vertices and %d edges:\n", graph->vertices, graph->edges);
    for (int i = 0; i < graph->vertices; i++) {
        printf("Vertex %d: ", i);
        int has_neighbors = 0;
        for (int j = 0; j < graph->vertices; j++) {
            if (graph->adjacency[i][j] != 0) {
                if (has_neighbors) printf(", ");
                printf("%d(%d)", j, graph->adjacency[i][j]);
                has_neighbors = 1;
            }
        }
        if (!has_neighbors) printf("No neighbors");
        printf("\n");
    }
}

int find_min_distance(int distances[], int visited[], int vertices) {
    int min_distance = INT_MAX;
    int min_index = -1;
    for (int v = 0; v < vertices; v++) {
        if (visited[v] == 0 && distances[v] <= min_distance) {
            min_distance = distances[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(struct Graph* graph, int start_vertex) {
    if (start_vertex < 0 || start_vertex >= graph->vertices) {
        printf("Invalid start vertex\n");
        return;
    }
    
    int distances[MAX_VERTICES];
    int visited[MAX_VERTICES];
    
    for (int i = 0; i < graph->vertices; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    
    distances[start_vertex] = 0;
    
    for (int count = 0; count < graph->vertices - 1; count++) {
        int u = find_min_distance(distances, visited, graph->vertices);
        if (u == -1) break;
        
        visited[u] = 1;
        
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && graph->adjacency[u][v] != 0 && distances[u] != INT_MAX) {
                int new_distance = distances[u] + graph->adjacency[u][v];
                if (new_distance > 0 && new_distance < distances[v]) {
                    distances[v] = new_distance;
                }
            }
        }
    }
    
    printf("Shortest distances from vertex %d:\n", start_vertex);
    for (int i = 0; i < graph->vertices; i++) {
        if (distances[i] == INT_MAX) {
            printf("Vertex %d: unreachable\n", i);
        } else {
            printf("Vertex %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int vertices, edges;
    
    printf("Enter number of vertices (1-%d): ", MAX_VERTICES);
    if (scanf("%d", &vertices) != 1 || vertices < 1 || vertices > MAX_VERTICES) {
        printf("Invalid number of vertices\n");
        return 1;
    }
    
    initialize_graph(&graph, vertices);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'source destination weight' (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge format\n");
            return 1;
        }
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge %d-%d\n", src, dest);
            return 1;
        }
    }
    
    printf("\n");
    print_graph(&graph);
    printf("\n");
    
    int start_vertex;
    printf("Enter start vertex for Dijkstra's algorithm