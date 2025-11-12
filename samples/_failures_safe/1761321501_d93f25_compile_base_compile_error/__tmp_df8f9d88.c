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
    int node;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* heap;
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
    heap->heap = malloc(capacity * sizeof(HeapNode));
    if (!heap->heap) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap_nodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int idx) {
    if (!heap || idx < 0 || idx >= heap->size) return;
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->heap[parent].dist <= heap->heap[idx].dist) break;
        swap_nodes(&heap->heap[parent], &heap->heap[idx]);
        idx = parent;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    if (!heap || idx < 0 || idx >= heap->size) return;
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < heap->size && heap->heap[left].dist < heap->heap[smallest].dist) {
        smallest = left;
    }
    if (right < heap->size && heap->heap[right].dist < heap->heap[smallest].dist) {
        smallest = right;
    }
    if (smallest != idx) {
        swap_nodes(&heap->heap[idx], &heap->heap[smallest]);
        heapify_down(heap, smallest);
    }
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (!heap || heap->size >= heap->capacity) return;
    heap->heap[heap->size].node = node;
    heap->heap[heap->size].dist = dist;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, -1};
    if (!heap || heap->size <= 0) return empty;
    HeapNode root = heap->heap[0];
    heap->heap[0] = heap->heap[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return root;
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

void dijkstra(Graph* graph, int start, int distances[]) {
    if (!graph || !distances) return;
    if (start < 0 || start >= graph->node_count) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap* heap = create_heap(graph->node_count);
    if (!heap) return;
    heap_push(heap, start, 0);
    
    while (heap->size > 0) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        if (u == -1) continue;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].u == u) {
                int v = graph->edges[i].v;
                int weight = graph->edges[i].weight;
                
                if (distances[u] != INT_MAX) {
                    if (weight > INT_MAX - distances[u]) continue;
                    int new_dist = distances[u] + weight;
                    if (new_dist >= 0 && new_dist < distances[v]) {
                        distances[v] = new_dist;
                        heap_push(heap, v, new_dist);
                    }
                }
            }
        }
    }
    
    free(heap->heap);
    free(heap);
}

int main() {
    Graph graph;
    int distances[MAX_NODES];
    init