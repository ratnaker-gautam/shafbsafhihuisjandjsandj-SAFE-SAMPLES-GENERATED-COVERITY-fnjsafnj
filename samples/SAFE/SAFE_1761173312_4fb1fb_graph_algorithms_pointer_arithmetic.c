//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int nodes[MAX_NODES];
    int edges[MAX_NODES][MAX_NODES];
    int node_count;
};

void init_graph(struct Graph *g) {
    g->node_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        g->nodes[i] = 0;
        for (int j = 0; j < MAX_NODES; j++) {
            *(*(g->edges + i) + j) = 0;
        }
    }
}

int add_node(struct Graph *g, int node_id) {
    if (g->node_count >= MAX_NODES || node_id < 0 || node_id >= MAX_NODES) {
        return 0;
    }
    if (*(g->nodes + node_id) == 1) {
        return 0;
    }
    *(g->nodes + node_id) = 1;
    g->node_count++;
    return 1;
}

int add_edge(struct Graph *g, int from, int to) {
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) {
        return 0;
    }
    if (*(g->nodes + from) == 0 || *(g->nodes + to) == 0) {
        return 0;
    }
    *(*(g->edges + from) + to) = 1;
    return 1;
}

void dfs(struct Graph *g, int node, int *visited) {
    if (node < 0 || node >= MAX_NODES || visited == NULL) {
        return;
    }
    if (*(visited + node) == 1) {
        return;
    }
    *(visited + node) = 1;
    printf("Visited node: %d\n", node);
    
    for (int i = 0; i < MAX_NODES; i++) {
        if (*(*(g->edges + node) + i) == 1 && *(visited + i) == 0) {
            dfs(g, i, visited);
        }
    }
}

void bfs(struct Graph *g, int start) {
    if (start < 0 || start >= MAX_NODES || *(g->nodes + start) == 0) {
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        *(visited + i) = 0;
    }
    
    int queue[MAX_NODES];
    int front = 0;
    int rear = 0;
    
    *(visited + start) = 1;
    *(queue + rear) = start;
    rear = (rear + 1) % MAX_NODES;
    
    while (front != rear) {
        int current = *(queue + front);
        front = (front + 1) % MAX_NODES;
        printf("Visited node: %d\n", current);
        
        for (int i = 0; i < MAX_NODES; i++) {
            if (*(*(g->edges + current) + i) == 1 && *(visited + i) == 0) {
                *(visited + i) = 1;
                *(queue + rear) = i;
                rear = (rear + 1) % MAX_NODES;
            }
        }
    }
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf("Adding nodes 0-4...\n");
    for (int i = 0; i < 5; i++) {
        if (!add_node(&g, i)) {
            printf("Failed to add node %d\n", i);
            return 1;
        }
    }
    
    printf("Adding edges...\n");
    int edges[][2] = {{0,1}, {0,2}, {1,3}, {2,4}, {3,4}};
    for (int i = 0; i < 5; i++) {
        if (!add_edge(&g, edges[i][0], edges[i][1])) {
            printf("Failed to add edge %d->%d\n", edges[i][0], edges[i][1]);
            return 1;
        }
    }
    
    printf("\nDFS starting from node 0:\n");
    int visited[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        *(visited + i) = 0;
    }
    dfs(&g, 0, visited);
    
    printf("\nBFS starting from node 0:\n");
    bfs(&g, 0);
    
    return 0;
}