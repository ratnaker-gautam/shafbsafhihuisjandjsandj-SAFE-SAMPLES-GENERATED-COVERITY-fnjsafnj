//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
} Graph;

int read_graph(Graph *graph) {
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &graph->node_count) != 1) {
        return 0;
    }
    if (graph->node_count < 1 || graph->node_count > MAX_NODES) {
        return 0;
    }

    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &graph->edge_count) != 1) {
        return 0;
    }
    if (graph->edge_count < 0 || graph->edge_count > MAX_EDGES) {
        return 0;
    }

    printf("Enter edges as 'from to weight' (nodes 0 to %d):\n", graph->node_count - 1);
    for (int i = 0; i < graph->edge_count; i++) {
        if (scanf("%d %d %d", &graph->edges[i].from, &graph->edges[i].to, &graph->edges[i].weight) != 3) {
            return 0;
        }
        if (graph->edges[i].from < 0 || graph->edges[i].from >= graph->node_count ||
            graph->edges[i].to < 0 || graph->edges[i].to >= graph->node_count) {
            return 0;
        }
    }
    return 1;
}

void print_graph(const Graph *graph) {
    printf("\nGraph with %d nodes and %d edges:\n", graph->node_count, graph->edge_count);
    for (int i = 0; i < graph->edge_count; i++) {
        printf("  %d -> %d (weight: %d)\n", 
               graph->edges[i].from, graph->edges[i].to, graph->edges[i].weight);
    }
}

int compare_edges(const void *a, const void *b) {
    const Edge *ea = (const Edge *)a;
    const Edge *eb = (const Edge *)b;
    if (ea->weight < eb->weight) return -1;
    if (ea->weight > eb->weight) return 1;
    return 0;
}

int find_parent(int parent[], int node) {
    if (parent[node] != node) {
        parent[node] = find_parent(parent, parent[node]);
    }
    return parent[node];
}

void kruskal_mst(const Graph *graph) {
    if (graph->node_count == 0 || graph->edge_count == 0) {
        printf("Graph is empty, no MST.\n");
        return;
    }

    Edge sorted_edges[MAX_EDGES];
    memcpy(sorted_edges, graph->edges, graph->edge_count * sizeof(Edge));
    qsort(sorted_edges, graph->edge_count, sizeof(Edge), compare_edges);

    int parent[MAX_NODES];
    for (int i = 0; i < graph->node_count; i++) {
        parent[i] = i;
    }

    Edge mst[MAX_NODES - 1];
    int mst_count = 0;
    int total_weight = 0;

    for (int i = 0; i < graph->edge_count && mst_count < graph->node_count - 1; i++) {
        int u = sorted_edges[i].from;
        int v = sorted_edges[i].to;
        int pu = find_parent(parent, u);
        int pv = find_parent(parent, v);

        if (pu != pv) {
            mst[mst_count] = sorted_edges[i];
            mst_count++;
            total_weight += sorted_edges[i].weight;
            parent[pu] = pv;
        }
    }

    if (mst_count != graph->node_count - 1) {
        printf("Graph is not connected, no complete MST.\n");
        return;
    }

    printf("\nMinimum Spanning Tree (Kruskal's algorithm):\n");
    printf("Total weight: %d\n", total_weight);
    for (int i = 0; i < mst_count; i++) {
        printf("  %d -> %d (weight: %d)\n", mst[i].from, mst[i].to, mst[i].weight);
    }
}

void bfs_traversal(const Graph *graph, int start) {
    if (start < 0 || start >= graph->node_count) {
        printf("Invalid start node for BFS.\n");
        return;
    }

    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;

    visited[start] = 1;
    queue[rear++] = start;

    printf("\nBFS traversal starting from node %d:\n", start);

    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);

        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == current && !visited[graph->edges[i].to]) {
                visited[graph->edges[i