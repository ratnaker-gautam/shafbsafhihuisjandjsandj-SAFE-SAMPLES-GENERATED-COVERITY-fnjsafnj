//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef void (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        return;
    }
    g->adjacency[from][to] = weight;
}

void print_shortest_paths(Graph *g, int source, int target) {
    if (source < 0 || source >= g->node_count || target < 0 || target >= g->node_count) {
        fprintf(stderr, "Invalid source or target\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INF;
        int min_index = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        visited[min_index] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_index][v] != INF && 
                dist[min_index] != INF && 
                dist[min_index] + g->adjacency[min_index][v] < dist[v]) {
                dist[v] = dist[min_index] + g->adjacency[min_index][v];
            }
        }
    }
    
    printf("Shortest distance from %d to %d: ", source, target);
    if (dist[target] == INF) {
        printf("No path exists\n");
    } else {
        printf("%d\n", dist[target]);
    }
}

void print_graph_info(Graph *g, int source, int target) {
    if (g->node_count <= 0 || g->node_count > MAX_NODES) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }
    
    printf("Graph with %d nodes\n", g->node_count);
    printf("Adjacency matrix:\n");
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            if (g->adjacency[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", g->adjacency[i][j]);
            }
        }
        printf("\n");
    }
}

void execute_operation(GraphOperation op, Graph *g, int a, int b) {
    if (op == NULL) {
        fprintf(stderr, "Invalid operation\n");
        return;
    }
    op(g, a, b);
}

int main() {
    Graph g;
    initialize_graph(&g, 5);
    
    add_edge(&g, 0, 1, 10);
    add_edge(&g, 0, 2, 5);
    add_edge(&g, 1, 2, 2);
    add_edge(&g, 1, 3, 1);
    add_edge(&g, 2, 1, 3);
    add_edge(&g, 2, 3, 9);
    add_edge(&g, 2, 4, 2);
    add_edge(&g, 3, 4, 4);
    add_edge(&g, 4, 3, 6);
    
    GraphOperation operations[] = {print_graph_info, print_shortest_paths};
    int op_count = sizeof(operations) / sizeof(operations[0]);
    
    for (int i = 0; i < op_count; i++) {
        if (i == 0) {
            execute_operation(operations[i], &g, 0, 0);
        } else {
            execute_operation(operations[i], &g, 0, 4);
        }
        printf("\n");
    }
    
    return 0;
}