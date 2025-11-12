//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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

void init_graph(struct Graph *graph, int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges <= 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    graph->edges = malloc(edges * sizeof(struct Edge));
    if (graph->edges == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void free_graph(struct Graph *graph) {
    if (graph->edges != NULL) {
        free(graph->edges);
        graph->edges = NULL;
    }
}

int validate_node(int node, int max_nodes) {
    return node >= 0 && node < max_nodes;
}

void add_edge(struct Graph *graph, int idx, int src, int dest, int weight) {
    if (idx < 0 || idx >= graph->num_edges) {
        fprintf(stderr, "Invalid edge index\n");
        return;
    }
    if (!validate_node(src, graph->num_nodes) || !validate_node(dest, graph->num_nodes)) {
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    struct Edge *edge_ptr = graph->edges + idx;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
}

void print_graph(struct Graph *graph) {
    printf("Graph with %d nodes and %d edges:\n", graph->num_nodes, graph->num_edges);
    struct Edge *edge_ptr = graph->edges;
    for (int i = 0; i < graph->num_edges; i++) {
        printf("Edge %d: %d -> %d (weight %d)\n", i, edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        edge_ptr++;
    }
}

void dfs_util(struct Graph *graph, int node, int *visited) {
    if (!validate_node(node, graph->num_nodes) || visited == NULL) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    struct Edge *edge_ptr = graph->edges;
    for (int i = 0; i < graph->num_edges; i++) {
        if (edge_ptr->src == node && !visited[edge_ptr->dest]) {
            dfs_util(graph, edge_ptr->dest, visited);
        }
        edge_ptr++;
    }
}

void dfs(struct Graph *graph, int start_node) {
    if (!validate_node(start_node, graph->num_nodes)) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES];
    memset(visited, 0, sizeof(visited));
    printf("DFS traversal from node %d: ", start_node);
    dfs_util(graph, start_node, visited);
    printf("\n");
}

void bfs(struct Graph *graph, int start_node) {
    if (!validate_node(start_node, graph->num_nodes)) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES];
    memset(visited, 0, sizeof(visited));
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start_node] = 1;
    queue[rear++] = start_node;
    printf("BFS traversal from node %d: ", start_node);
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        struct Edge *edge_ptr = graph->edges;
        for (int i = 0; i < graph->num_edges; i++) {
            if (edge_ptr->src == current && !visited[edge_ptr->dest]) {
                visited[edge_ptr->dest] = 1;
                queue[rear++] = edge_ptr->dest;
            }
            edge_ptr++;
        }
    }
    printf("\n");
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    printf("Enter number of edges (1-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1 || num_edges < 1 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    init_graph(&graph, num_nodes, num_edges);
    printf("Enter edges as 'source destination weight' (0-indexed):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            free_graph(&graph);
            return