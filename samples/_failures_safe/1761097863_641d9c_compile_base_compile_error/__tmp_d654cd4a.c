//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int num_nodes;
    int num_edges;
    struct Edge *edges;
};

void init_graph(struct Graph *g, int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges <= 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    g->edges = malloc(edges * sizeof(struct Edge));
    if (g->edges == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void free_graph(struct Graph *g) {
    free(g->edges);
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *e1 = (const struct Edge*)a;
    const struct Edge *e2 = (const struct Edge*)b;
    return e1->weight - e2->weight;
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

void kruskal_mst(struct Graph *g) {
    if (g == NULL || g->edges == NULL) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }
    
    struct Edge *result = malloc((g->num_nodes - 1) * sizeof(struct Edge));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    qsort(g->edges, g->num_edges, sizeof(struct Edge), compare_edges);
    
    int *parent = malloc(g->num_nodes * sizeof(int));
    int *rank = malloc(g->num_nodes * sizeof(int));
    if (parent == NULL || rank == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(result);
        free(parent);
        free(rank);
        return;
    }
    
    for (int i = 0; i < g->num_nodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    int e = 0;
    int i = 0;
    
    while (e < g->num_nodes - 1 && i < g->num_edges) {
        struct Edge next_edge = *(g->edges + i);
        i++;
        
        int x = find_parent(parent, next_edge.src);
        int y = find_parent(parent, next_edge.dest);
        
        if (x != y) {
            *(result + e) = next_edge;
            e++;
            union_sets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        struct Edge edge = *(result + j);
        printf("%d - %d : %d\n", edge.src, edge.dest, edge.weight);
        total_weight += edge.weight;
    }
    printf("Total weight: %d\n", total_weight);
    
    free(result);
    free(parent);
    free(rank);
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (1-%d): ", MAX_EDGES);
    if (scanf("%d", &edges) != 1 || edges < 1 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    init_graph(&g, nodes, edges);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            free_graph(&g);
            return 1;
        }
        if (src < 0 || src >= nodes || dest < 0 || dest >= nodes) {
            fprintf(stderr, "Invalid node index\n");
            free_graph(&g);
            return 1;
        }
        if (weight < 0) {
            fprintf(stderr, "Invalid weight\n");
            free_graph(&g);
            return 1;
        }
        (g.edges + i)->src = src;
        (