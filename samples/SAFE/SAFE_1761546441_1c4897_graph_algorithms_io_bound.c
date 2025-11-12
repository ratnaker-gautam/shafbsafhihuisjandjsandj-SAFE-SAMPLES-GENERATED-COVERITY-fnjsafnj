//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int edge_weights[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
            g->edge_weights[i][j] = 0;
        }
    }
}

int add_node(struct Graph *g) {
    if (g->num_nodes >= MAX_NODES) {
        return -1;
    }
    return g->num_nodes++;
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->num_nodes || to < 0 || to >= g->num_nodes) {
        return -1;
    }
    if (g->adjacency[from][to] == 1) {
        return -1;
    }
    g->adjacency[from][to] = 1;
    g->edge_weights[from][to] = weight;
    g->num_edges++;
    return 0;
}

void print_graph(struct Graph *g) {
    printf("Graph has %d nodes and %d edges\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d: ", i);
        int has_edges = 0;
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->adjacency[i][j]) {
                printf("%d(w:%d) ", j, g->edge_weights[i][j]);
                has_edges = 1;
            }
        }
        if (!has_edges) {
            printf("no outgoing edges");
        }
        printf("\n");
    }
}

void bfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        printf("Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal from node %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[current][i] && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    
    printf("Graph Construction\n");
    printf("Adding nodes...\n");
    
    for (int i = 0; i < 6; i++) {
        int node_id = add_node(&g);
        if (node_id == -1) {
            printf("Failed to add node\n");
            return 1;
        }
        printf("Added node %d\n", node_id);
    }
    
    printf("\nAdding edges...\n");
    int edges[][3] = {{0, 1, 5}, {0, 2, 3}, {1, 3, 2}, {2, 3, 1}, {3, 4, 4}, {4, 5, 2}};
    int num_edges = sizeof(edges) / sizeof(edges[0]);
    
    for (int i = 0; i < num_edges; i++) {
        if (add_edge(&g, edges[i][0], edges[i][1], edges[i][2]) == -1) {
            printf("Failed to add edge from %d to %d\n", edges[i][0], edges[i][1]);
            return 1;
        }
        printf("Added edge %d->%d (weight: %d)\n", edges[i][0], edges[i][1], edges[i][2]);
    }
    
    printf("\n");
    print_graph(&g);
    
    printf("\n");
    bfs(&g, 0);
    
    printf("\nEnter a start node for BFS (0-%d): ", g.num_nodes - 1);
    int user_start;
    if (scanf("%d", &user_start) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    bfs(&g, user_start);
    
    return 0;
}