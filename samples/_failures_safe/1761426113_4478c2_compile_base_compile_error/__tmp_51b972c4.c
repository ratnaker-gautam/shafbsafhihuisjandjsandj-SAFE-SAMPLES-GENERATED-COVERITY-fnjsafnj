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
    if (g == NULL || from < 0 || from >= g->node_count || 
        to < 0 || to >= g->node_count || weight < 0) {
        fprintf(stderr, "Invalid edge parameters\n");
        return;
    }
    g->adjacency[from][to] = weight;
}

void print_shortest_paths(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || 
        end < 0 || end >= g->node_count) {
        fprintf(stderr, "Invalid node indices\n");
        return;
    }

    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[start] = 0;

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
                dist[min_index] != INF) {
                long new_dist = (long)dist[min_index] + g->adjacency[min_index][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }

    printf("Shortest distance from %d to %d: ", start, end);
    if (dist[end] == INF) {
        printf("No path exists\n");
    } else {
        printf("%d\n", dist[end]);
    }
}

void find_connected_components(Graph *g, int start, int end) {
    if (g == NULL) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }

    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = -1;
    int component_count = 0;

    for (int i = 0; i < g->node_count; i++) {
        if (!visited[i]) {
            component_count++;
            printf("Component %d: ", component_count);
            
            top = -1;
            stack[++top] = i;
            visited[i] = 1;

            while (top >= 0) {
                int current = stack[top--];
                printf("%d ", current);

                for (int j = 0; j < g->node_count; j++) {
                    if ((g->adjacency[current][j] != INF || 
                         g->adjacency[j][current] != INF) && !visited[j]) {
                        visited[j] = 1;
                        stack[++top] = j;
                    }
                }
            }
            printf("\n");
        }
    }
    printf("Total connected components: %d\n", component_count);
}

void execute_graph_operation(Graph *g, int start, int end, GraphOperation op) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid operation parameters\n");
        return;
    }
    op(g, start, end);
}

int main() {
    Graph g;
    int choice, nodes, edges, from, to, weight;

    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    initialize_graph(&g, nodes);

    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * nodes) {
        fprintf(stderr, "Invalid edge count\n");
        return 1;
    }

    for (int i = 0; i < edges; i++) {
        printf("Enter edge %d (from to weight): ", i + 1);
        if (scanf("%d %d %d", &from, &to, &weight) != 3 || 
            from < 0 || from >= nodes || to <