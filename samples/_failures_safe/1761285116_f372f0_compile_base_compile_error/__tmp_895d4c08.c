//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int u, v, weight;
} Edge;

typedef struct {
    int node;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* array;
    int size;
    int capacity;
} MinHeap;

typedef struct {
    Edge edges[MAX_EDGES];
    int edge_count;
    int node_count;
} Graph;

MinHeap* create_min_heap(int capacity) {
    if (capacity <= 0) return NULL;
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->array = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    if (!heap->array) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap_heap_nodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(MinHeap* heap, int idx) {
    if (!heap || idx < 0 || idx >= heap->size) return;
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->array[left].dist < heap->array[smallest].dist) {
        smallest = left;
    }
    if (right < heap->size && heap->array[right].dist < heap->array[smallest].dist) {
        smallest = right;
    }
    if (smallest != idx) {
        swap_heap_nodes(&heap->array[idx], &heap->array[smallest]);
        min_heapify(heap, smallest);
    }
}

HeapNode extract_min(MinHeap* heap) {
    HeapNode empty = {-1, INT_MAX};
    if (!heap || heap->size <= 0) return empty;
    if (heap->size == 1) {
        heap->size--;
        return heap->array[0];
    }
    HeapNode root = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    min_heapify(heap, 0);
    return root;
}

void decrease_key(MinHeap* heap, int node, int dist) {
    if (!heap || node < 0) return;
    int i;
    for (i = 0; i < heap->size; i++) {
        if (heap->array[i].node == node) {
            heap->array[i].dist = dist;
            while (i != 0 && heap->array[(i - 1) / 2].dist > heap->array[i].dist) {
                swap_heap_nodes(&heap->array[i], &heap->array[(i - 1) / 2]);
                i = (i - 1) / 2;
            }
            break;
        }
    }
}

int is_in_heap(MinHeap* heap, int node) {
    if (!heap || node < 0) return 0;
    for (int i = 0; i < heap->size; i++) {
        if (heap->array[i].node == node) {
            return 1;
        }
    }
    return 0;
}

void init_graph(Graph* graph, int nodes) {
    if (!graph || nodes <= 0 || nodes > MAX_NODES) {
        if (graph) {
            graph->node_count = 0;
            graph->edge_count = 0;
        }
        return;
    }
    graph->node_count = nodes;
    graph->edge_count = 0;
}

int add_edge(Graph* graph, int u, int v, int weight) {
    if (!graph || graph->edge_count >= MAX_EDGES) return 0;
    if (u < 0 || u >= graph->node_count) return 0;
    if (v < 0 || v >= graph->node_count) return 0;
    if (weight < 0) return 0;
    graph->edges[graph->edge_count].u = u;
    graph->edges[graph->edge_count].v = v;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(Graph* graph, int src) {
    if (!graph || graph->node_count <= 0 || src < 0 || src >= graph->node_count) return;
    
    int dist[MAX_NODES];
    MinHeap* heap = create_min_heap(graph->node_count);
    if (!heap) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        dist[i] = INT_MAX;
        heap->array[i].node = i;
        heap->array[i].dist = INT_MAX;
    }
    heap->size = graph->node_count;
    dist[src] = 0;
    decrease_key(heap, src, 0);
    
    while (heap->size > 0) {
        HeapNode min_node = extract_min(heap);
        int u = min_node.node;
        if (u == -1) break;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].u == u) {
                int