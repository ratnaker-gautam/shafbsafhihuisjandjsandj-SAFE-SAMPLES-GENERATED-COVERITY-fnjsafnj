//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int visited[MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
        }
        g->visited[i] = 0;
    }
}

int add_edge(struct Graph *g, int from, int to) {
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) {
        return 0;
    }
    if (g->adjacency[from][to] == 0) {
        g->adjacency[from][to] = 1;
        g->num_edges++;
        return 1;
    }
    return 0;
}

void dfs(struct Graph *g, int node) {
    if (node < 0 || node >= MAX_NODES || g->visited[node]) {
        return;
    }
    g->visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < MAX_NODES; i++) {
        if (g->adjacency[node][i] && !g->visited[i]) {
            dfs(g, i);
        }
    }
}

void reset_visited(struct Graph *g) {
    for (int i = 0; i < MAX_NODES; i++) {
        g->visited[i] = 0;
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int node_count;
    if (scanf("%d", &node_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    graph.num_nodes = node_count;
    
    printf("Enter number of edges: ");
    int edge_count;
    if (scanf("%d", &edge_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (edge_count < 0 || edge_count > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'from to' pairs (0-indexed):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (from < 0 || from >= node_count || to < 0 || to >= node_count) {
            printf("Invalid node index\n");
            return 1;
        }
        if (!add_edge(&graph, from, to)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    printf("Enter starting node for DFS: ");
    int start_node;
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (start_node < 0 || start_node >= node_count) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    printf("DFS traversal: ");
    dfs(&graph, start_node);
    printf("\n");
    
    reset_visited(&graph);
    
    printf("BFS traversal: ");
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    queue[rear++] = start_node;
    graph.visited[start_node] = 1;
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < MAX_NODES; i++) {
            if (graph.adjacency[current][i] && !graph.visited[i]) {
                if (rear >= MAX_NODES) {
                    printf("Queue overflow\n");
                    return 1;
                }
                queue[rear++] = i;
                graph.visited[i] = 1;
            }
        }
    }
    printf("\n");
    
    return 0;
}