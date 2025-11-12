//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Edge {
    int source;
    int destination;
    int weight;
};

struct Graph {
    int vertex_count;
    int edge_count;
    struct Edge* edges;
};

struct Graph* create_graph(int vertices, int edges) {
    if (vertices <= 0 || edges < 0) return NULL;
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->vertex_count = vertices;
    graph->edge_count = edges;
    graph->edges = malloc(edges * sizeof(struct Edge));
    if (!graph->edges) {
        free(graph);
        return NULL;
    }
    return graph;
}

void destroy_graph(struct Graph* graph) {
    if (!graph) return;
    free(graph->edges);
    free(graph);
}

int validate_edge(const struct Edge* edge, int max_vertex) {
    if (!edge) return 0;
    if (edge->source < 0 || edge->source >= max_vertex) return 0;
    if (edge->destination < 0 || edge->destination >= max_vertex) return 0;
    return 1;
}

void bellman_ford(struct Graph* graph, int source) {
    if (!graph || source < 0 || source >= graph->vertex_count) return;
    
    int* distances = malloc(graph->vertex_count * sizeof(int));
    if (!distances) return;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;
    
    for (int i = 1; i < graph->vertex_count; i++) {
        for (int j = 0; j < graph->edge_count; j++) {
            if (!validate_edge(&graph->edges[j], graph->vertex_count)) continue;
            int u = graph->edges[j].source;
            int v = graph->edges[j].destination;
            int weight = graph->edges[j].weight;
            
            if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
            }
        }
    }
    
    for (int j = 0; j < graph->edge_count; j++) {
        if (!validate_edge(&graph->edges[j], graph->vertex_count)) continue;
        int u = graph->edges[j].source;
        int v = graph->edges[j].destination;
        int weight = graph->edges[j].weight;
        
        if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
            printf("Graph contains negative weight cycle\n");
            free(distances);
            return;
        }
    }
    
    printf("Vertex distances from source %d:\n", source);
    for (int i = 0; i < graph->vertex_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("%d: INFINITY\n", i);
        } else {
            printf("%d: %d\n", i, distances[i]);
        }
    }
    
    free(distances);
}

int main() {
    int vertices = 5;
    int edges = 8;
    struct Graph* graph = create_graph(vertices, edges);
    if (!graph) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    struct Edge edge_list[] = {
        {0, 1, -1}, {0, 2, 4}, {1, 2, 3}, {1, 3, 2},
        {1, 4, 2}, {3, 2, 5}, {3, 1, 1}, {4, 3, -3}
    };
    
    for (int i = 0; i < edges; i++) {
        if (!validate_edge(&edge_list[i], vertices)) {
            printf("Invalid edge at index %d\n", i);
            destroy_graph(graph);
            return 1;
        }
        graph->edges[i] = edge_list[i];
    }
    
    bellman_ford(graph, 0);
    
    destroy_graph(graph);
    return 0;
}