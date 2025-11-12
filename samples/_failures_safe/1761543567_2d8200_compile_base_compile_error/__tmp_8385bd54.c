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
    
    if (graph[src].size >= graph[src].capacity) {
        int new_cap = graph[src].capacity == 0 ? 4 : graph[src].capacity * 2;
        if (new_cap > MAX_NODES * 2) new_cap = MAX_NODES * 2;
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

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int idx) {
    if (idx <= 0) return;
    int parent = (idx - 1) / 2;
    if (heap->nodes[parent].dist > heap->nodes[idx].dist) {
        heap_swap(&heap->nodes[parent], &heap->nodes[idx]);
        heapify_up(heap, parent);
    }
}

void heapify_down(MinHeap* heap, int idx) {
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    int smallest = idx;
    
    if (left < heap->size && heap->nodes[left].dist < heap->nodes[smallest].dist) {
        smallest = left;
    }
    if (right < heap->size && heap->nodes[right].dist < heap->nodes[smallest].dist) {
        smallest = right;
    }
    
    if (smallest != idx) {
        heap_swap(&heap->nodes[idx], &heap->nodes[smallest]);
        heapify_down(heap, smallest);
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
    HeapNode empty = {-1, -1};
    if (!heap || heap->size == 0) return empty;
    
    HeapNode result = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return result;
}

void dijkstra(int start, int distances[]) {
    if (start < 0 || start >= node_count) return;
    
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap* heap = create_heap(node_count);
    if (!heap) return;
    
    heap_push(heap, start, 0);
    
    while (heap->size > 0) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        
        if (visited[u]) continue;
        visited[u] = 1;
        
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].edges[i].dest;
            int weight = graph[u].edges[i].weight;
            
            if (weight < 0) continue;
            
            if (distances[u] != INT_MAX) {
                long new_dist = (long)distances[u] + weight;
                if (new_dist > INT_MAX) continue;
                if (new_dist