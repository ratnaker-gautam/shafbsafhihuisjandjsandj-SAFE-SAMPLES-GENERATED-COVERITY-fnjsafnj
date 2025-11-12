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

void initializeGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            if (i == j) {
                g->matrix[i][j] = 0;
            } else {
                g->matrix[i][j] = INF;
            }
        }
    }
}

int addEdge(struct Graph* g, int from, int to, int weight) {
    if (g == NULL) return 0;
    if (from < 0 || from >= g->nodes) return 0;
    if (to < 0 || to >= g->nodes) return 0;
    if (weight < 0) return 0;
    g->matrix[from][to] = weight;
    return 1;
}

void printGraph(struct Graph* g) {
    if (g == NULL) return;
    printf("Graph adjacency matrix:\n");
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            if (g->matrix[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", g->matrix[i][j]);
            }
        }
        printf("\n");
    }
}

void dijkstra(struct Graph* g, int source) {
    if (g == NULL) return;
    if (source < 0 || source >= g->nodes) return;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int minDist = INF;
        int minNode = -1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minNode = v;
            }
        }
        
        if (minNode == -1) break;
        visited[minNode] = 1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->matrix[minNode][v] != INF && 
                dist[minNode] != INF && 
                dist[minNode] + g->matrix[minNode][v] < dist[v]) {
                dist[v] = dist[minNode] + g->matrix[minNode][v];
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int getValidInput(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph graph;
    int nodes, edges;
    
    printf("Graph Algorithms Demo\n");
    printf("====================\n\n");
    
    nodes = getValidInput("Enter number of nodes (1-100): ", 1, MAX_NODES);
    initializeGraph(&graph, nodes);
    
    edges = getValidInput("Enter number of edges to add: ", 0, nodes * nodes);
    
    for (int i = 0; i < edges; i++) {
        printf("\nEdge %d:\n", i + 1);
        int from = getValidInput("From node: ", 0, nodes - 1);
        int to = getValidInput("To node: ", 0, nodes - 1);
        int weight = getValidInput("Weight (0-1000): ", 0, 1000);
        
        if (!addEdge(&graph, from, to, weight)) {
            printf("Failed to add edge. Please try again.\n");
            i--;
        }
    }
    
    printf("\n");
    printGraph(&graph);
    printf("\n");
    
    int source = getValidInput("Enter source node for Dijkstra algorithm: ", 0, nodes - 1);
    dijkstra(&graph, source);
    
    return 0;
}