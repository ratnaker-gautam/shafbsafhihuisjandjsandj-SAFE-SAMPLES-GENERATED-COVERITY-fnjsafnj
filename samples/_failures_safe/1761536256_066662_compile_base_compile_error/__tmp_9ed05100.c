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
        if (new_cap > MAX_NODES) new_cap = MAX_NODES;
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

MinHeap* create_heap(int capacity) {
    if (capacity <= 0 || capacity > MAX_NODES) return NULL;
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->nodes = malloc(capacity * sizeof(HeapNode));
    if (!heap->nodes) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heapify_up(MinHeap* heap, int idx) {
    if (!heap || idx < 0 || idx >= heap->size) return;
    int parent = (idx - 1) / 2;
    while (idx > 0 && heap->nodes[parent].dist > heap->nodes[idx].dist) {
        HeapNode temp = heap->nodes[parent];
        heap->nodes[parent] = heap->nodes[idx];
        heap->nodes[idx] = temp;
        idx = parent;
        parent = (idx - 1) / 2;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    if (!heap || idx < 0 || idx >= heap->size) return;
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < heap->size && heap->nodes[left].dist < heap->nodes[smallest].dist) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right].dist < heap->nodes[smallest].dist) {
            smallest = right;
        }
        if (smallest == idx) break;
        
        HeapNode temp = heap->nodes[idx];
        heap->nodes[idx] = heap->nodes[smallest];
        heap->nodes[smallest] = temp;
        idx = smallest;
    }
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (!heap || heap->size >= heap->capacity) return;
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].dist = dist;
    heap->size++;
    heapify_up(heap, heap->size - 1);
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, INT_MAX};
    if (!heap || heap->size == 0) return empty;
    HeapNode result = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return result;
}

void dijkstra(int start) {
    if (start < 0 || start >= node_count) return;
    
    int dist[MAX_NODES];
    for (int i = 0; i < node_count; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    MinHeap* heap = create_heap(node_count);
    if (!heap) return;
    heap_push(heap, start, 0);
    
    while (heap->size > 0) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].edges[i].dest;
            int weight = graph[u].edges[i].weight;
            
            if (weight < 0) continue;
            if (dist[u] == INT_MAX) continue;
            
            long new_dist = (long)dist[u] + weight;