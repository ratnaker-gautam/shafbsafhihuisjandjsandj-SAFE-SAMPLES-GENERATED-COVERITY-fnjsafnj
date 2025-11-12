//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    HeapNode* elements;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_heap(int capacity) {
    if (capacity <= 0) return NULL;
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

void free_heap(MinHeap* heap) {
    if (heap) {
        free(heap->elements);
        free(heap);
    }
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
        if (heap->elements[parent].dist <= heap->elements[idx].dist) break;
        swap_nodes(&heap->elements[parent], &heap->elements[idx]);
        idx = parent;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    if (!heap || idx < 0 || idx >= heap->size) return;
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < heap->size && heap->elements[left].dist < heap->elements[smallest].dist) {
            smallest = left;
        }
        if (right < heap->size && heap->elements[right].dist < heap->elements[smallest].dist) {
            smallest = right;
        }
        if (smallest == idx) break;
        swap_nodes(&heap->elements[idx], &heap->elements[smallest]);
        idx = smallest;
    }
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (!heap || heap->size >= heap->capacity) return;
    heap->elements[heap->size].node = node;
    heap->elements[heap->size].dist = dist;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode result = {0, INT_MAX};
    if (!heap || heap->size <= 0) return result;
    result = heap->elements[0];
    heap->size--;
    if (heap->size > 0) {
        heap->elements[0] = heap->elements[heap->size];
        heapify_down(heap, 0);
    }
    return result;
}

int is_heap_empty(MinHeap* heap) {
    return !heap || heap->size == 0;
}

int validate_graph_input(int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES) return 0;
    if (edges < 0 || edges > MAX_EDGES) return 0;
    if (edges > nodes * (nodes - 1) / 2) return 0;
    return 1;
}

int validate_edge(int u, int v, int weight, int nodes) {
    if (u < 0 || u >= nodes) return 0;
    if (v < 0 || v >= nodes) return 0;
    if (u == v) return 0;
    if (weight < 0 || weight > 1000) return 0;
    return 1;
}

void dijkstra(int nodes, Edge* edges, int edge_count, int start) {
    if (nodes <= 0 || !edges || edge_count < 0 || start < 0 || start >= nodes) return;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    MinHeap* heap = create_heap(nodes);
    if (!heap) return;
    heap_push(heap, start, 0);
    
    while (!is_heap_empty(heap)) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        
        if (visited[u]) continue;
        visited[u] = 1;
        
        for (int i = 0; i < edge_count; i++) {
            if (edges[i].u == u) {
                int v = edges[i].v;
                int weight = edges[i].weight;
                
                if (dist[u] != INT_MAX) {
                    long long new_dist = (long long)dist[u] + weight;
                    if (new_dist > 0 && new_dist < INT_MAX && new_dist < dist[v]) {
                        dist[v] = (int)new_dist;
                        heap_push(heap, v, dist[v]);
                    }
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for