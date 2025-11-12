//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 500
#define MAX_LINE 1024

struct Graph {
    int nodes[MAX_NODES];
    int edges[MAX_NODES][MAX_NODES];
    int node_count;
    int edge_count;
};

void init_graph(struct Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        g->nodes[i] = 0;
        for (int j = 0; j < MAX_NODES; j++) {
            g->edges[i][j] = 0;
        }
    }
}

int add_node(struct Graph *g, int node_id) {
    if (g->node_count >= MAX_NODES) return 0;
    if (node_id < 0 || node_id >= MAX_NODES) return 0;
    if (g->nodes[node_id]) return 0;
    
    g->nodes[node_id] = 1;
    g->node_count++;
    return 1;
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) return 0;
    if (!g->nodes[from] || !g->nodes[to]) return 0;
    if (g->edges[from][to] != 0) return 0;
    if (g->edge_count >= MAX_EDGES) return 0;
    
    g->edges[from][to] = weight;
    g->edge_count++;
    return 1;
}

void bfs(struct Graph *g, int start) {
    if (start < 0 || start >= MAX_NODES || !g->nodes[start]) return;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal from node %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < MAX_NODES; i++) {
            if (g->edges[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
}

void dfs_util(struct Graph *g, int node, int visited[]) {
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < MAX_NODES; i++) {
        if (g->edges[node][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= MAX_NODES || !g->nodes[start]) return;
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfs_util(g, start, visited);
    printf("\n");
}

int read_int(const char *prompt, int min, int max) {
    char buffer[MAX_LINE];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter an integer between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph graph;
    init_graph(&graph);
    
    printf("Graph Algorithms Demo\n");
    printf("====================\n\n");
    
    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Add node\n");
        printf("2. Add edge\n");
        printf("3. BFS traversal\n");
        printf("4. DFS traversal\n");
        printf("5. Exit\n");
        
        choice = read_int("Enter your choice (1-5): ", 1, 5);
        if (choice == -1) break;
        
        switch (choice) {
            case 1: {
                int node = read_int("Enter node ID (0-99): ", 0, 99);
                if (node != -1) {
                    if (add_node(&graph, node)) {
                        printf("Node %d added successfully.\n", node);
                    } else {
                        printf("Failed to add node %d.\n", node);
                    }
                }
                break;
            }
            case 2: {
                int from = read_int("Enter source node: ", 0, 99);
                if (from == -1) break;
                int to = read_int("Enter destination node: ", 0, 99);
                if (to == -1) break;
                int weight = read_int("Enter edge weight: ", -1000, 1000);
                if (weight == -1) break;
                
                if (add