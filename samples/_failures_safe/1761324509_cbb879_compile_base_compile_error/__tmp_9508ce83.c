//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int num_vertices;
    int num_edges;
    struct Edge* edges;
};

struct AdjListNode {
    int dest;
    struct AdjListNode* next;
};

struct AdjList {
    struct AdjListNode* head;
};

struct AdjGraph {
    int num_vertices;
    struct AdjList* array;
};

struct AdjListNode* create_adj_list_node(int dest) {
    struct AdjListNode* new_node = malloc(sizeof(struct AdjListNode));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->dest = dest;
    new_node->next = NULL;
    return new_node;
}

struct AdjGraph* create_adj_graph(int num_vertices) {
    if (num_vertices <= 0) {
        return NULL;
    }
    struct AdjGraph* graph = malloc(sizeof(struct AdjGraph));
    if (graph == NULL) {
        return NULL;
    }
    graph->num_vertices = num_vertices;
    graph->array = malloc(num_vertices * sizeof(struct AdjList));
    if (graph->array == NULL) {
        free(graph);
        return NULL;
    }
    for (int i = 0; i < num_vertices; i++) {
        graph->array[i].head = NULL;
    }
    return graph;
}

void add_adj_edge(struct AdjGraph* graph, int src, int dest) {
    if (graph == NULL || src < 0 || src >= graph->num_vertices || dest < 0 || dest >= graph->num_vertices) {
        return;
    }
    struct AdjListNode* new_node = create_adj_list_node(dest);
    if (new_node == NULL) {
        return;
    }
    new_node->next = graph->array[src].head;
    graph->array[src].head = new_node;
}

void dfs_util(struct AdjGraph* graph, int vertex, int* visited) {
    if (graph == NULL || visited == NULL || vertex < 0 || vertex >= graph->num_vertices) {
        return;
    }
    visited[vertex] = 1;
    printf("%d ", vertex);
    struct AdjListNode* current = graph->array[vertex].head;
    while (current != NULL) {
        if (!visited[current->dest]) {
            dfs_util(graph, current->dest, visited);
        }
        current = current->next;
    }
}

void perform_dfs(struct AdjGraph* graph, int start_vertex) {
    if (graph == NULL || start_vertex < 0 || start_vertex >= graph->num_vertices) {
        return;
    }
    int* visited = calloc(graph->num_vertices, sizeof(int));
    if (visited == NULL) {
        return;
    }
    printf("DFS traversal starting from vertex %d: ", start_vertex);
    dfs_util(graph, start_vertex, visited);
    printf("\n");
    free(visited);
}

struct Graph* create_graph(int num_vertices, int num_edges) {
    if (num_vertices <= 0 || num_edges < 0) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->num_vertices = num_vertices;
    graph->num_edges = num_edges;
    graph->edges = malloc(num_edges * sizeof(struct Edge));
    if (graph->edges == NULL) {
        free(graph);
        return NULL;
    }
    return graph;
}

int compare_edges(const void* a, const void* b) {
    const struct Edge* edge_a = (const struct Edge*)a;
    const struct Edge* edge_b = (const struct Edge*)b;
    if (edge_a->weight < edge_b->weight) return -1;
    if (edge_a->weight > edge_b->weight) return 1;
    return 0;
}

int find_parent(int parent[], int i) {
    if (parent[i] == i) {
        return i;
    }
    return find_parent(parent, parent[i]);
}

void union_sets(int parent[], int rank[], int x, int y) {
    int x_root = find_parent(parent, x);
    int y_root = find_parent(parent, y);
    if (rank[x_root] < rank[y_root]) {
        parent[x_root] = y_root;
    } else if (rank[x_root] > rank[y_root]) {
        parent[y_root] = x_root;
    } else {
        parent[y_root] = x_root;
        rank[x_root]++;
    }
}

void kruskal_mst(struct Graph* graph) {
    if (graph == NULL || graph->num_vertices <= 0 || graph->num_edges <= 0) {
        return;
    }
    struct Edge* result = malloc((graph->num_vertices - 1) * sizeof(struct Edge));
    if (result == NULL) {
        return;
    }
    int* parent = malloc(graph->num_vertices * sizeof(int));
    if (parent == NULL) {
        free(result);
        return;
    }
    int* rank = calloc(graph->num_vertices, sizeof(int));
    if (rank == NULL) {
        free(result);
        free(parent);
        return;
    }
    for (int v = 0; v < graph->num_vertices; v++) {
        parent[v] = v;
    }
    qsort(graph->edges, graph->num_edges, sizeof(struct Edge), compare_edges);