//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int u, v, weight;
} Edge;

typedef struct {
    int node;
    int distance;
} HeapNode;

typedef struct {
    HeapNode* elements;
    int size;
    int capacity;
} MinHeap;

typedef struct {
    Edge edges[MAX_EDGES];
    int edge_count;
    int node_count;
} Graph;

MinHeap* create_heap(int capacity) {
    if (capacity <= 0) capacity = 1;
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->elements = malloc(capacity * sizeof(HeapNode));
    if (!heap->elements) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heapify_up(MinHeap* heap, int index) {
    if (!heap || index < 0 || index >= heap->size) return;
    if (index == 0) return;
    int parent = (index - 1) / 2;
    if (heap->elements[index].distance < heap->elements[parent].distance) {
        HeapNode temp = heap->elements[index];
        heap->elements[index] = heap->elements[parent];
        heap->elements[parent] = temp;
        heapify_up(heap, parent);
    }
}

void heapify_down(MinHeap* heap, int index) {
    if (!heap || index < 0 || index >= heap->size) return;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;
    
    if (left < heap->size && heap->elements[left].distance < heap->elements[smallest].distance) {
        smallest = left;
    }
    if (right < heap->size && heap->elements[right].distance < heap->elements[smallest].distance) {
        smallest = right;
    }
    if (smallest != index) {
        HeapNode temp = heap->elements[index];
        heap->elements[index] = heap->elements[smallest];
        heap->elements[smallest] = temp;
        heapify_down(heap, smallest);
    }
}

void heap_push(MinHeap* heap, int node, int distance) {
    if (!heap || heap->size >= heap->capacity) return;
    heap->elements[heap->size].node = node;
    heap->elements[heap->size].distance = distance;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, -1};
    if (!heap || heap->size == 0) return empty;
    HeapNode result = heap->elements[0];
    heap->elements[0] = heap->elements[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return result;
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

void dijkstra(Graph* graph, int start) {
    if (!graph) return;
    if (start < 0 || start >= graph->node_count) return;
    
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = -1;
        visited[i] = 0;
    }
    distances[start] = 0;
    
    MinHeap* heap = create_heap(MAX_NODES);
    if (!heap) return;
    heap_push(heap, start, 0);
    
    while (heap->size > 0) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        if (u == -1) continue;
        if (visited[u]) continue;
        visited[u] = 1;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].u == u) {
                int v = graph->edges[i].v;
                int weight = graph->edges[i].weight;
                if (v < 0 || v >= graph->node_count) continue;
                if (!visited[v]) {
                    if (distances[u] != -1) {
                        int new_dist = distances[u] + weight;
                        if (distances[v] == -1 || new_dist < distances[v]) {
                            distances[v] = new_dist;
                            heap_push(heap, v, new_dist);
                        }