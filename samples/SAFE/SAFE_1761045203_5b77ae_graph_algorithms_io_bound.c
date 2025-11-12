//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

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
    g->nodes[g->node_count++] = node_id;
    return 1;
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= MAX_NODES) return 0;
    if (to < 0 || to >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    if (g->edges[from][to] != 0) return 0;
    g->edges[from][to] = weight;
    g->edge_count++;
    return 1;
}

void bfs(struct Graph *g, int start) {
    if (start < 0 || start >= MAX_NODES) return;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal: ");
    
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
    if (node < 0 || node >= MAX_NODES) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < MAX_NODES; i++) {
        if (g->edges[node][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= MAX_NODES) return;
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf("Graph Algorithms Demo\n");
    printf("Adding nodes 0-9...\n");
    
    for (int i = 0; i < 10; i++) {
        if (!add_node(&g, i)) {
            printf("Failed to add node %d\n", i);
            return 1;
        }
    }
    
    printf("Adding edges...\n");
    int edges[][3] = {{0,1,1}, {0,2,1}, {1,3,1}, {1,4,1}, {2,5,1}, {2,6,1}, {3,7,1}, {4,8,1}, {5,9,1}};
    int edge_count = sizeof(edges) / sizeof(edges[0]);
    
    for (int i = 0; i < edge_count; i++) {
        if (!add_edge(&g, edges[i][0], edges[i][1], edges[i][2])) {
            printf("Failed to add edge %d->%d\n", edges[i][0], edges[i][1]);
            return 1;
        }
    }
    
    printf("Graph has %d nodes and %d edges\n", g.node_count, g.edge_count);
    
    int start_node;
    printf("Enter starting node for traversals (0-9): ");
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (start_node < 0 || start_node > 9) {
        printf("Node must be between 0 and 9\n");
        return 1;
    }
    
    bfs(&g, start_node);
    dfs(&g, start_node);
    
    return 0;
}