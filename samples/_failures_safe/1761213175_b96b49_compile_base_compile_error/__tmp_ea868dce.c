//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000
#define MAX_LINE 1024

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int num_nodes;
    int num_edges;
    struct Edge edges[MAX_EDGES];
};

void read_graph(struct Graph *graph) {
    char buffer[MAX_LINE];
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        exit(1);
    }
    if (sscanf(buffer, "%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        exit(1);
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        exit(1);
    }
    if (sscanf(buffer, "%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        exit(1);
    }
    
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    
    printf("Enter edges as 'source destination weight' (0-indexed nodes):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        
        printf("Edge %d: ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            exit(1);
        }
        if (sscanf(buffer, "%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge format\n");
            exit(1);
        }
        if (src < 0 || src >= nodes || dest < 0 || dest >= nodes) {
            printf("Invalid node index\n");
            exit(1);
        }
        
        graph->edges[i].src = src;
        graph->edges[i].dest = dest;
        graph->edges[i].weight = weight;
    }
}

int compare_edges(const void *a, const void *b) {
    struct Edge *edge1 = (struct Edge *)a;
    struct Edge *edge2 = (struct Edge *)b;
    return edge1->weight - edge2->weight;
}

int find_parent(int parent[], int node) {
    if (parent[node] != node) {
        parent[node] = find_parent(parent, parent[node]);
    }
    return parent[node];
}

void union_sets(int parent[], int rank[], int x, int y) {
    int root_x = find_parent(parent, x);
    int root_y = find_parent(parent, y);
    
    if (root_x != root_y) {
        if (rank[root_x] < rank[root_y]) {
            parent[root_x] = root_y;
        } else if (rank[root_x] > rank[root_y]) {
            parent[root_y] = root_x;
        } else {
            parent[root_y] = root_x;
            rank[root_x]++;
        }
    }
}

void kruskal_mst(struct Graph *graph) {
    if (graph->num_nodes == 0 || graph->num_edges == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int edge_count = 0;
    int i = 0;
    
    for (int v = 0; v < graph->num_nodes; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    qsort(graph->edges, graph->num_edges, sizeof(struct Edge), compare_edges);
    
    for (i = 0; i < graph->num_edges && edge_count < graph->num_nodes - 1; i++) {
        struct Edge next_edge = graph->edges[i];
        
        int x = find_parent(parent, next_edge.src);
        int y = find_parent(parent, next_edge.dest);
        
        if (x != y) {
            result[edge_count] = next_edge;
            edge_count++;
            union_sets(parent, rank, x, y);
        }
    }
    
    printf("\nMinimum Spanning Tree (Kruskal's algorithm):\n");
    int total_weight = 0;
    for (i = 0; i < edge_count; i++) {
        printf("Edge %d-%d with weight %d\n", result[i].src, result[i].dest, result[i].weight);
        total_weight += result[i].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

void bfs_traversal(struct Graph *graph) {
    if (graph->num_nodes == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int start_node;
    char buffer[MAX_LINE];
    
    printf("Enter starting node for BFS (0-%d): ", graph->num_nodes - 1);
    if (fgets(buffer, sizeof(buffer