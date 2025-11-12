//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

struct Graph* createGraph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adj[i][j] = 0;
        }
    }
    return graph;
}

int addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || src >= graph->nodes || dest < 0 || dest >= graph->nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    graph->adj[src][dest] = weight;
    graph->adj[dest][src] = weight;
    return 1;
}

int minDistance(int dist[], int visited[], int nodes) {
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
        int u = minDistance(dist, visited, graph->nodes);
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->adj[u][v] != 0 && dist[u] != INT_MAX) {
                int new_dist = dist[u] + graph->adj[u][v];
                if (new_dist > 0 && new_dist < dist[v]) {
                    dist[v] = new_dist;
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

int validateInput(int min, int max) {
    int value;
    while (1) {
        if (scanf("%d", &value) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Enter a number between %d and %d: ", min, max);
            continue;
        }
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value out of range. Enter a number between %d and %d: ", min, max);
    }
}

int main() {
    printf("Graph Algorithms: Dijkstra's Shortest Path\n");
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int nodes = validateInput(1, MAX_NODES);
    struct Graph* graph = createGraph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph.\n");
        return 1;
    }
    printf("Enter number of edges: ");
    int edges = validateInput(0, nodes * (nodes - 1) / 2);
    for (int i = 0; i < edges; i++) {
        printf("Edge %d: enter source, destination, weight: ", i + 1);
        int src = validateInput(0, nodes - 1);
        int dest = validateInput(0, nodes - 1);
        int weight = validateInput(0, 1000);
        if (!addEdge(graph, src, dest, weight)) {
            printf("Failed to add edge.\n");
            i--;
        }
    }
    printf("Enter source vertex for Dijkstra (0-%d): ", nodes - 1);
    int src = validateInput(0, nodes - 1);
    dijkstra(graph, src);
    free(graph);
    return 0;
}