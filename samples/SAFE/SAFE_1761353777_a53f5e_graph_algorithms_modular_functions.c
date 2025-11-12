//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int matrix[MAX_NODES][MAX_NODES];
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
            graph->matrix[i][j] = (i == j) ? 0 : INF;
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
    graph->matrix[src][dest] = weight;
    graph->matrix[dest][src] = weight;
    return 1;
}

int findMinDistance(int dist[], int visited[], int nodes) {
    int min = INF;
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
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;
    for (int count = 0; count < graph->nodes - 1; count++) {
        int u = findMinDistance(dist, visited, graph->nodes);
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->matrix[u][v] != INF && dist[u] != INF && 
                dist[u] + graph->matrix[u][v] < dist[v]) {
                dist[v] = dist[u] + graph->matrix[u][v];
            }
        }
    }
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
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
    printf("Graph Algorithm Demo\n");
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int nodes = validateInput(1, MAX_NODES);
    struct Graph* graph = createGraph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    printf("Enter number of edges: ");
    int edges = validateInput(0, nodes * (nodes - 1) / 2);
    for (int i = 0; i < edges; i++) {
        printf("Edge %d - enter source, destination, weight: ", i + 1);
        int src = validateInput(0, nodes - 1);
        int dest = validateInput(0, nodes - 1);
        int weight = validateInput(0, 1000);
        if (!addEdge(graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            i--;
        }
    }
    printf("Enter source node for Dijkstra's algorithm: ");
    int src = validateInput(0, nodes - 1);
    dijkstra(graph, src);
    free(graph);
    return 0;
}