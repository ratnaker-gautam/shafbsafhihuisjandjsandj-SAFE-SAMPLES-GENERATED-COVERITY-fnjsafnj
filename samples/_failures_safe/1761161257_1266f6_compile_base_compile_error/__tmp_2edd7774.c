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
    int node_count;
} Graph;

typedef struct {
    int node;
    int distance;
} HeapNode;

typedef struct {
    HeapNode* nodes;
    int size;
    int capacity;
} MinHeap;

void init_graph(Graph* graph, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        node_count = MAX_NODES;
    }
    graph->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        graph->lists[i].edges = NULL;
        graph->lists[i].size = 0;
        graph->lists[i].capacity = 0;
    }
}

void add_edge(Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->node_count || dest < 0 || dest >= graph->node_count) {
        return;
    }
    if (weight < 0) {
        weight = 0;
    }
    
    AdjList* list = &graph->lists[src];
    if (list->size >= list->capacity) {
        int new_capacity = list->capacity == 0 ? 4 : list->capacity * 2;
        if (new_capacity > MAX_NODES * 2) {
            new_capacity = MAX_NODES * 2;
        }
        Edge* new_edges = realloc(list->edges, new_capacity * sizeof(Edge));
        if (new_edges) {
            list->edges = new_edges;
            list->capacity = new_capacity;
        } else {
            return;
        }
    }
    
    list->edges[list->size].dest = dest;
    list->edges[list->size].weight = weight;
    list->size++;
}

void init_heap(MinHeap* heap, int capacity) {
    if (capacity <= 0) {
        capacity = MAX_NODES;
    }
    heap->nodes = malloc(capacity * sizeof(HeapNode));
    if (heap->nodes) {
        heap->size = 0;
        heap->capacity = capacity;
    } else {
        heap->size = 0;
        heap->capacity = 0;
    }
}

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->nodes[index].distance >= heap->nodes[parent].distance) {
            break;
        }
        heap_swap(&heap->nodes[index], &heap->nodes[parent]);
        index = parent;
    }
}

void heapify_down(MinHeap* heap, int index) {
    int left, right, smallest;
    while (1) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        smallest = index;
        
        if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == index) {
            break;
        }
        heap_swap(&heap->nodes[index], &heap->nodes[smallest]);
        index = smallest;
    }
}

void heap_push(MinHeap* heap, int node, int distance) {
    if (heap->size >= heap->capacity) {
        return;
    }
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].distance = distance;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, INT_MAX};
    if (heap->size <= 0) {
        return empty;
    }
    HeapNode result = heap->nodes[0];
    heap->size--;
    if (heap->size > 0) {
        heap->nodes[0] = heap->nodes[heap->size];
        heapify_down(heap, 0);
    }
    return result;
}

void dijkstra(Graph* graph, int start, int distances[]) {
    if (start < 0 || start >= graph->node_count) {
        return;
    }
    
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap heap;
    init_heap(&heap, graph->node_count);
    if (heap.nodes == NULL) {
        return;
    }
    
    heap_push(&heap, start, 0);
    
    while (heap.size > 0) {
        HeapNode current = heap_pop(&heap);
        if (current.distance > distances[current.node]) {
            continue;
        }
        
        AdjList* list = &graph->lists[current.node];
        for (int i = 0; i < list->size; i++) {
            Edge edge =