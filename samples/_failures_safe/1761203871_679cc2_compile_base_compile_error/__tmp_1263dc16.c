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

void add_edge(Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->node_count || v < 0 || v >= g->node_count) {
        fprintf(stderr, "Invalid edge nodes\n");
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        return;
    }
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
}

void dijkstra_operation(Graph *g, int source, int target) {
    if (source < 0 || source >= g->node_count || target < 0 || target >= g->node_count) {
        fprintf(stderr, "Invalid source or target\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
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
    
    if (dist[target] != INF) {
        printf("Shortest path from %d to %d: %d\n", source, target, dist[target]);
    } else {
        printf("No path exists from %d to %d\n", source, target);
    }
}

void bfs_operation(Graph *g, int source, int target) {
    if (source < 0 || source >= g->node_count || target < 0 || target >= g->node_count) {
        fprintf(stderr, "Invalid source or target\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int parent[MAX_NODES];
    int path[MAX_NODES];
    int path_length = 0;
    
    for (int i = 0; i < g->node_count; i++) {
        parent[i] = -1;
    }
    
    visited[source] = 1;
    queue[rear++] = source;
    
    while (front < rear) {
        int current = queue[front++];
        
        if (current == target) {
            int node = target;
            while (node != -1) {
                path[path_length++] = node;
                node = parent[node];
            }
            
            printf("BFS path from %d to %d: ", source, target);
            for (int i = path_length - 1; i >= 0; i--) {
                printf("%d", path[i]);
                if (i > 0) printf(" -> ");
            }
            printf("\n");
            return;
        }
        
        for (int v = 0; v < g->node_count; v++) {
            if (g->adjacency[current][v] != INF && !visited[v]) {
                visited[v] = 1;
                parent[v] = current;
                queue[rear++] = v;
            }
        }
    }
    
    printf("No BFS path found from %d to %d\n", source, target);
}

void execute_graph_operation(Graph *g, GraphOperation op, int source, int target) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid graph or operation\n");
        return;
    }
    op(g, source, target);
}

int main() {
    Graph g;
    initialize_graph(&g, 6);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1