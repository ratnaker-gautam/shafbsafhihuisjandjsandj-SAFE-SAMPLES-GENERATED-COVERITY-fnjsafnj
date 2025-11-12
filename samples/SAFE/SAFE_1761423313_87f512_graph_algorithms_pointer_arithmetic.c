//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes[MAX_NODES];
    int edges[MAX_EDGES][3];
    int node_count;
    int edge_count;
};

void init_graph(struct Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
}

int add_node(struct Graph *g, int id) {
    if (g->node_count >= MAX_NODES) return 0;
    if (id < 0 || id >= MAX_NODES) return 0;
    
    int *nodes = g->nodes;
    for (int i = 0; i < g->node_count; i++) {
        if (*(nodes + i) == id) return 0;
    }
    
    *(nodes + g->node_count) = id;
    g->node_count++;
    return 1;
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (g->edge_count >= MAX_EDGES) return 0;
    if (from < 0 || from >= MAX_NODES) return 0;
    if (to < 0 || to >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    
    int (*edges)[3] = g->edges;
    for (int i = 0; i < g->edge_count; i++) {
        if (*(*(edges + i) + 0) == from && *(*(edges + i) + 1) == to) {
            return 0;
        }
    }
    
    *(*(edges + g->edge_count) + 0) = from;
    *(*(edges + g->edge_count) + 1) = to;
    *(*(edges + g->edge_count) + 2) = weight;
    g->edge_count++;
    return 1;
}

void dfs(struct Graph *g, int start, int *visited) {
    if (start < 0 || start >= MAX_NODES) return;
    if (*(visited + start)) return;
    
    *(visited + start) = 1;
    printf("%d ", start);
    
    int (*edges)[3] = g->edges;
    for (int i = 0; i < g->edge_count; i++) {
        if (*(*(edges + i) + 0) == start) {
            int neighbor = *(*(edges + i) + 1);
            if (!*(visited + neighbor)) {
                dfs(g, neighbor, visited);
            }
        }
    }
}

void bfs(struct Graph *g, int start) {
    if (start < 0 || start >= MAX_NODES) return;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    *(visited + start) = 1;
    *(queue + rear) = start;
    rear++;
    
    printf("BFS: ");
    
    while (front < rear) {
        int current = *(queue + front);
        front++;
        printf("%d ", current);
        
        int (*edges)[3] = g->edges;
        for (int i = 0; i < g->edge_count; i++) {
            if (*(*(edges + i) + 0) == current) {
                int neighbor = *(*(edges + i) + 1);
                if (!*(visited + neighbor)) {
                    *(visited + neighbor) = 1;
                    *(queue + rear) = neighbor;
                    rear++;
                    if (rear >= MAX_NODES) rear = MAX_NODES - 1;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf("Graph Operations:\n");
    printf("Adding nodes 0-4...\n");
    for (int i = 0; i < 5; i++) {
        if (!add_node(&g, i)) {
            printf("Failed to add node %d\n", i);
            return 1;
        }
    }
    
    printf("Adding edges...\n");
    int edges[][3] = {{0,1,1}, {0,2,1}, {1,3,1}, {2,4,1}, {3,4,1}};
    for (int i = 0; i < 5; i++) {
        if (!add_edge(&g, edges[i][0], edges[i][1], edges[i][2])) {
            printf("Failed to add edge %d->%d\n", edges[i][0], edges[i][1]);
            return 1;
        }
    }
    
    printf("DFS: ");
    int visited[MAX_NODES] = {0};
    dfs(&g, 0, visited);
    printf("\n");
    
    bfs(&g, 0);
    
    printf("Adding invalid node -1...\n");
    if (!add_node(&g, -1)) {
        printf("Correctly rejected invalid node\n");
    }
    
    printf("Adding duplicate edge 0->1...\n");
    if (!add_edge(&g, 0, 1, 1)) {
        printf("Correctly rejected duplicate edge\n");
    }
    
    return 0;
}