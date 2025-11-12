//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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
    struct Edge edges[MAX_EDGES];
};

void initialize_graph(struct Graph *graph) {
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) {
        return 0;
    }
    if (src < 0 || dest < 0 || src >= MAX_NODES || dest >= MAX_NODES) {
        return 0;
    }
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    if (src >= graph->num_nodes) graph->num_nodes = src + 1;
    if (dest >= graph->num_nodes) graph->num_nodes = dest + 1;
    return 1;
}

int compare_edges(const void *a, const void *b) {
    struct Edge *edgeA = (struct Edge *)a;
    struct Edge *edgeB = (struct Edge *)b;
    return edgeA->weight - edgeB->weight;
}

int find_parent(int parent[], int node) {
    if (parent[node] != node) {
        parent[node] = find_parent(parent, parent[node]);
    }
    return parent[node];
}

void union_sets(int parent[], int rank[], int x, int y) {
    int rootX = find_parent(parent, x);
    int rootY = find_parent(parent, y);
    if (rootX != rootY) {
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}

void kruskal_mst(struct Graph *graph) {
    if (graph->num_nodes == 0 || graph->num_edges == 0) {
        printf("Graph is empty.\n");
        return;
    }

    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    int e = 0;
    int i = 0;

    for (int v = 0; v < graph->num_nodes; v++) {
        parent[v] = v;
        rank[v] = 0;
    }

    qsort(graph->edges, graph->num_edges, sizeof(struct Edge), compare_edges);

    while (e < graph->num_nodes - 1 && i < graph->num_edges) {
        struct Edge next_edge = graph->edges[i++];
        int x = find_parent(parent, next_edge.src);
        int y = find_parent(parent, next_edge.dest);

        if (x != y) {
            result[e++] = next_edge;
            union_sets(parent, rank, x, y);
        }
    }

    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        printf("%d -- %d weight: %d\n", result[j].src, result[j].dest, result[j].weight);
        total_weight += result[j].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter an integer between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);

    printf("Graph Minimum Spanning Tree (Kruskal's Algorithm)\n");
    printf("Enter graph edges (source destination weight). Use -1 to finish.\n");

    while (1) {
        int src = read_int("Enter source node (0-99): ", -1, 99);
        if (src == -1) break;

        int dest = read_int("Enter destination node (0-99): ", 0, 99);
        int weight = read_int("Enter edge weight (1-1000): ", 1, 1000);

        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge. Maximum edges reached or invalid nodes.\n");
            break;
        }

        char buffer[10];
        printf("Add another edge? (y/n): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        if (buffer[0] != 'y' && buffer[0] != 'Y') break;
    }

    kruskal_mst(&graph);
    return 0;
}