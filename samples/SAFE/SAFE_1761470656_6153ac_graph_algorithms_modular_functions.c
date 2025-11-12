//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes;
    int edges[MAX_EDGES][3];
    int edge_count;
};

struct Graph* create_graph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->nodes = nodes;
    graph->edge_count = 0;
    return graph;
}

int add_edge(struct Graph* graph, int from, int to, int weight) {
    if (graph == NULL || from < 0 || from >= graph->nodes || to < 0 || to >= graph->nodes) {
        return 0;
    }
    if (graph->edge_count >= MAX_EDGES) {
        return 0;
    }
    graph->edges[graph->edge_count][0] = from;
    graph->edges[graph->edge_count][1] = to;
    graph->edges[graph->edge_count][2] = weight;
    graph->edge_count++;
    return 1;
}

int find_min_distance(int dist[], int visited[], int nodes) {
    int min = INT_MAX;
    int min_index = -1;
    for (int v = 0; v < nodes; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(struct Graph* graph, int src) {
    if (graph == NULL || src < 0 || src >= graph->nodes) {
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    for (int count = 0; count < graph->nodes - 1; count++) {
        int u = find_min_distance(dist, visited, graph->nodes);
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i][0] == u) {
                int v = graph->edges[i][1];
                int weight = graph->edges[i][2];
                if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                }
            }
        }
    }
    printf("Vertex Distance from Source %d\n", src);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d \t INF\n", i);
        } else {
            printf("%d \t %d\n", i, dist[i]);
        }
    }
}

int validate_input(int min, int max, const char* prompt) {
    int value;
    printf("%s", prompt);
    if (scanf("%d", &value) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    if (value < min || value > max) {
        return -1;
    }
    return value;
}

int main() {
    int nodes = validate_input(1, MAX_NODES, "Enter number of nodes (1-100): ");
    if (nodes == -1) {
        printf("Invalid number of nodes.\n");
        return 1;
    }
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph.\n");
        return 1;
    }
    int edges = validate_input(0, MAX_EDGES, "Enter number of edges: ");
    if (edges == -1) {
        printf("Invalid number of edges.\n");
        free(graph);
        return 1;
    }
    for (int i = 0; i < edges; i++) {
        printf("Edge %d: ", i + 1);
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge format.\n");
            while (getchar() != '\n');
            free(graph);
            return 1;
        }
        if (!add_edge(graph, from, to, weight)) {
            printf("Failed to add edge.\n");
            free(graph);
            return 1;
        }
    }
    int src = validate_input(0, nodes - 1, "Enter source vertex: ");
    if (src == -1) {
        printf("Invalid source vertex.\n");
        free(graph);
        return 1;
    }
    dijkstra(graph, src);
    free(graph);
    return 0;
}