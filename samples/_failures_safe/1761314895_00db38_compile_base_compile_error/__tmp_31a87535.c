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
    int distance;
} HeapNode;

typedef struct {
    HeapNode* nodes;
    int size;
    int capacity;
} MinHeap;

void init_graph(Graph* graph, int num_nodes) {
    if (num_nodes <= 0 || num_nodes > MAX_NODES) {
        num_nodes = MAX_NODES;
    }
    graph->num_nodes = num_nodes;
    for (int i = 0; i < num_nodes; i++) {
        graph->lists[i].edges = NULL;
        graph->lists[i].size = 0;
        graph->lists[i].capacity = 0;
    }
}

void add_edge(Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->num_nodes || dest < 0 || dest >= graph->num_nodes) {
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

void init_heap(MinHeap* heap, int capacity) {
    if (capacity <= 0) {
        capacity = MAX_NODES;
    }
    heap->nodes = malloc(capacity * sizeof(HeapNode));
    if (heap->nodes == NULL) {
        heap->capacity = 0;
        heap->size = 0;
        return;
    }
    heap->capacity = capacity;
    heap->size = 0;
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
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;
        
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
    heap->size++;
    heapify_up(heap, heap->size - 1);
}

int heap_pop(MinHeap* heap) {
    if (heap->size == 0) {
        return -1;
    }
    int node = heap->nodes[0].node;
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return node;
}

void dijkstra(Graph* graph, int start) {
    if (start < 0 || start >= graph->num_nodes) {
        return;
    }
    
    int distances[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap heap;
    init_heap(&heap, graph->num_nodes);
    if (heap.nodes == NULL) {
        return;
    }
    
    heap_push(&heap, start, 0);
    
    while (heap.size > 0) {
        int current = heap_pop(&heap);
        if (current == -1) {
            break;
        }
        
        AdjList* list = &graph->lists[current];
        for (int i = 0; i < list->size; i++) {
            int neighbor = list->edges[i].dest;
            int weight = list->edges[i].weight;
            int new_dist =