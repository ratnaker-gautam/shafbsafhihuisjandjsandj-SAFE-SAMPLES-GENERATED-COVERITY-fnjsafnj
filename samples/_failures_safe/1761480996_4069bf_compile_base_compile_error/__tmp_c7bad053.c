//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int u, v, weight;
} Edge;

typedef struct {
    Edge* edges;
    int size;
    int capacity;
} MinHeap;

typedef struct {
    Edge edges[MAX_EDGES];
    int edge_count;
    int node_count;
} Graph;

MinHeap* create_heap(int capacity) {
    if (capacity <= 0) return NULL;
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->edges = malloc(capacity * sizeof(Edge));
    if (!heap->edges) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heapify_up(MinHeap* heap, int idx) {
    if (!heap || idx <= 0) return;
    int parent = (idx - 1) / 2;
    if (heap->edges[idx].weight < heap->edges[parent].weight) {
        Edge temp = heap->edges[idx];
        heap->edges[idx] = heap->edges[parent];
        heap->edges[parent] = temp;
        heapify_up(heap, parent);
    }
}

void heapify_down(MinHeap* heap, int idx) {
    if (!heap) return;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    int smallest = idx;
    
    if (left < heap->size && heap->edges[left].weight < heap->edges[smallest].weight) {
        smallest = left;
    }
    if (right < heap->size && heap->edges[right].weight < heap->edges[smallest].weight) {
        smallest = right;
    }
    if (smallest != idx) {
        Edge temp = heap->edges[idx];
        heap->edges[idx] = heap->edges[smallest];
        heap->edges[smallest] = temp;
        heapify_down(heap, smallest);
    }
}

void heap_push(MinHeap* heap, Edge edge) {
    if (!heap || heap->size >= heap->capacity) return;
    heap->edges[heap->size] = edge;
    heapify_up(heap, heap->size);
    heap->size++;
}

Edge heap_pop(MinHeap* heap) {
    Edge empty = {0, 0, INT_MAX};
    if (!heap || heap->size <= 0) return empty;
    Edge root = heap->edges[0];
    heap->edges[0] = heap->edges[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return root;
}

void free_heap(MinHeap* heap) {
    if (heap) {
        free(heap->edges);
        free(heap);
    }
}

void init_graph(Graph* graph, int nodes) {
    if (!graph) return;
    if (nodes <= 0 || nodes > MAX_NODES) nodes = MAX_NODES;
    graph->node_count = nodes;
    graph->edge_count = 0;
}

int add_edge(Graph* graph, int u, int v, int weight) {
    if (!graph) return 0;
    if (u < 0 || u >= graph->node_count || v < 0 || v >= graph->node_count) return 0;
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    graph->edges[graph->edge_count].u = u;
    graph->edges[graph->edge_count].v = v;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

int find_parent(int parent[], int node) {
    if (parent[node] != node) {
        parent[node] = find_parent(parent, parent[node]);
    }
    return parent[node];
}

int union_sets(int parent[], int rank[], int u, int v) {
    int root_u = find_parent(parent, u);
    int root_v = find_parent(parent, v);
    
    if (root_u == root_v) return 0;
    
    if (rank[root_u] < rank[root_v]) {
        parent[root_u] = root_v;
    } else if (rank[root_u] > rank[root_v]) {
        parent[root_v] = root_u;
    } else {
        parent[root_v] = root_u;
        rank[root_u]++;
    }
    return 1;
}

void kruskal_mst(Graph* graph) {
    if (!graph || graph->node_count <= 0 || graph->edge_count <= 0) return;
    
    MinHeap* heap = create_heap(graph->edge_count);
    if (!heap) return;
    
    for (int i = 0; i < graph->edge_count; i++) {
        heap_push(heap, graph->edges[i]);
    }
    
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    for (int i = 0; i < graph->node_count; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    int edges_used = 0;
    int total_weight = 0;
    
    printf("Minimum Spanning Tree edges:\n");
    while (edges_used < graph->node_count - 1