//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int dest;
    int weight;
} Edge;

typedef struct {
    Edge* edges;
    int size;
    int capacity;
} AdjList;

typedef struct {
    int node;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* nodes;
    int size;
    int capacity;
} MinHeap;

AdjList graph[MAX_NODES];
int node_count;

void init_graph() {
    for (int i = 0; i < MAX_NODES; i++) {
        graph[i].edges = NULL;
        graph[i].size = 0;
        graph[i].capacity = 0;
    }
    node_count = 0;
}

void add_edge(int src, int dest, int weight) {
    if (src < 0 || src >= MAX_NODES || dest < 0 || dest >= MAX_NODES) return;
    if (weight < 0) return;
    
    if (graph[src].size >= graph[src].capacity) {
        int new_cap = graph[src].capacity == 0 ? 4 : graph[src].capacity * 2;
        if (new_cap > MAX_NODES * 2) return;
        Edge* new_edges = realloc(graph[src].edges, new_cap * sizeof(Edge));
        if (!new_edges) return;
        graph[src].edges = new_edges;
        graph[src].capacity = new_cap;
    }
    
    graph[src].edges[graph[src].size].dest = dest;
    graph[src].edges[graph[src].size].weight = weight;
    graph[src].size++;
    
    if (src >= node_count) node_count = src + 1;
    if (dest >= node_count) node_count = dest + 1;
}

void init_heap(MinHeap* heap, int capacity) {
    heap->nodes = malloc(capacity * sizeof(HeapNode));
    heap->size = 0;
    heap->capacity = capacity;
}

void swap_nodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->nodes[parent].dist <= heap->nodes[idx].dist) break;
        swap_nodes(&heap->nodes[parent], &heap->nodes[idx]);
        idx = parent;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < heap->size && heap->nodes[left].dist < heap->nodes[smallest].dist)
            smallest = left;
        if (right < heap->size && heap->nodes[right].dist < heap->nodes[smallest].dist)
            smallest = right;
        
        if (smallest == idx) break;
        swap_nodes(&heap->nodes[idx], &heap->nodes[smallest]);
        idx = smallest;
    }
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (heap->size >= heap->capacity) return;
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].dist = dist;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode result = heap->nodes[0];
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    heapify_down(heap, 0);
    return result;
}

int is_heap_empty(MinHeap* heap) {
    return heap->size == 0;
}

void dijkstra(int start, int dist[]) {
    if (start < 0 || start >= node_count) return;
    
    for (int i = 0; i < node_count; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    MinHeap heap;
    init_heap(&heap, node_count * 2);
    heap_push(&heap, start, 0);
    
    while (!is_heap_empty(&heap)) {
        HeapNode current = heap_pop(&heap);
        int u = current.node;
        
        if (current.dist != dist[u]) continue;
        
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].edges[i].dest;
            int weight = graph[u].edges[i].weight;
            
            if (weight < 0) continue;
            
            if (dist[u] != INT_MAX) {
                long new_dist = (long)dist[u] + weight;
                if (new_dist < INT_MAX && new_dist < dist[v]) {
                    dist[v] = (int)new_dist;
                    heap_push(&heap, v, dist[v]);
                }
            }
        }
    }
    
    free(heap.nodes);
}

void free_graph() {
    for (int i = 0; i < MAX_NODES; i++) {
        if (graph[i].edges != NULL) {
            free(graph[i].edges);
            graph[i].edges = NULL;
        }
        graph[i].size = 0;
        graph[i