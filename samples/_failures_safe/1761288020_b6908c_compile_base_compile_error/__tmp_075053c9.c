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
    AdjList lists[MAX_NODES];
    int num_nodes;
} Graph;

typedef struct {
    int node;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* nodes;
    int size;
    int capacity;
} MinHeap;

void init_graph(Graph* g, int num_nodes) {
    if (num_nodes <= 0 || num_nodes > MAX_NODES) {
        num_nodes = MAX_NODES;
    }
    g->num_nodes = num_nodes;
    for (int i = 0; i < num_nodes; i++) {
        g->lists[i].edges = NULL;
        g->lists[i].size = 0;
        g->lists[i].capacity = 0;
    }
}

void add_edge(Graph* g, int src, int dest, int weight) {
    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        return;
    }
    if (weight < 0) {
        return;
    }
    
    AdjList* list = &g->lists[src];
    if (list->size >= list->capacity) {
        int new_capacity = list->capacity == 0 ? 4 : list->capacity * 2;
        if (new_capacity > MAX_NODES * 2) {
            new_capacity = MAX_NODES * 2;
        }
        Edge* new_edges = realloc(list->edges, new_capacity * sizeof(Edge));
        if (new_edges == NULL) {
            return;
        }
        list->edges = new_edges;
        list->capacity = new_capacity;
    }
    
    list->edges[list->size].dest = dest;
    list->edges[list->size].weight = weight;
    list->size++;
}

MinHeap* create_heap(int capacity) {
    if (capacity <= 0) {
        capacity = MAX_NODES;
    }
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (heap == NULL) {
        return NULL;
    }
    heap->nodes = malloc(capacity * sizeof(HeapNode));
    if (heap->nodes == NULL) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heapify_up(MinHeap* heap, int index) {
    if (heap == NULL || index < 0 || index >= heap->size) {
        return;
    }
    HeapNode temp = heap->nodes[index];
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->nodes[parent].dist <= temp.dist) {
            break;
        }
        heap->nodes[index] = heap->nodes[parent];
        index = parent;
    }
    heap->nodes[index] = temp;
}

void heapify_down(MinHeap* heap, int index) {
    if (heap == NULL || index < 0 || index >= heap->size) {
        return;
    }
    HeapNode temp = heap->nodes[index];
    while (index * 2 + 1 < heap->size) {
        int child = index * 2 + 1;
        if (child + 1 < heap->size && heap->nodes[child + 1].dist < heap->nodes[child].dist) {
            child++;
        }
        if (temp.dist <= heap->nodes[child].dist) {
            break;
        }
        heap->nodes[index] = heap->nodes[child];
        index = child;
    }
    heap->nodes[index] = temp;
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (heap == NULL || heap->size >= heap->capacity) {
        return;
    }
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].dist = dist;
    heap->size++;
    heapify_up(heap, heap->size - 1);
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, INT_MAX};
    if (heap == NULL || heap->size == 0) {
        return empty;
    }
    HeapNode result = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return result;
}

void dijkstra(Graph* g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        return;
    }
    
    int dist[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    MinHeap* heap = create_heap(g->num_nodes);
    if (heap == NULL) {
        return;
    }
    heap_push(heap, start, 0);
    
    while (heap->size > 0) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        
        if (current.dist != dist[u]) {
            continue;
        }
        
        for (int i = 0; i