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
    AdjList* array;
    int numNodes;
} Graph;

typedef struct {
    int node;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

Graph* createGraph(int numNodes) {
    if (numNodes <= 0 || numNodes > MAX_NODES) return NULL;
    Graph* graph = malloc(sizeof(Graph));
    if (!graph) return NULL;
    graph->numNodes = numNodes;
    graph->array = malloc(numNodes * sizeof(AdjList));
    if (!graph->array) {
        free(graph);
        return NULL;
    }
    for (int i = 0; i < numNodes; i++) {
        graph->array[i].edges = NULL;
        graph->array[i].size = 0;
        graph->array[i].capacity = 0;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) return;
    if (weight < 0) return;
    
    AdjList* list = &graph->array[src];
    if (list->size >= list->capacity) {
        int newCapacity = list->capacity == 0 ? 4 : list->capacity * 2;
        if (newCapacity > MAX_NODES) newCapacity = MAX_NODES;
        Edge* newEdges = realloc(list->edges, newCapacity * sizeof(Edge));
        if (!newEdges) return;
        list->edges = newEdges;
        list->capacity = newCapacity;
    }
    
    list->edges[list->size].dest = dest;
    list->edges[list->size].weight = weight;
    list->size++;
}

MinHeap* createMinHeap(int capacity) {
    if (capacity <= 0 || capacity > MAX_NODES) return NULL;
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->data = malloc(capacity * sizeof(HeapNode));
    if (!heap->data) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int idx) {
    if (idx <= 0) return;
    int parent = (idx - 1) / 2;
    if (heap->data[parent].dist > heap->data[idx].dist) {
        swapHeapNodes(&heap->data[parent], &heap->data[idx]);
        heapifyUp(heap, parent);
    }
}

void heapifyDown(MinHeap* heap, int idx) {
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    int smallest = idx;
    
    if (left < heap->size && heap->data[left].dist < heap->data[smallest].dist) {
        smallest = left;
    }
    if (right < heap->size && heap->data[right].dist < heap->data[smallest].dist) {
        smallest = right;
    }
    if (smallest != idx) {
        swapHeapNodes(&heap->data[idx], &heap->data[smallest]);
        heapifyDown(heap, smallest);
    }
}

void insertHeap(MinHeap* heap, int node, int dist) {
    if (heap->size >= heap->capacity) return;
    heap->data[heap->size].node = node;
    heap->data[heap->size].dist = dist;
    heapifyUp(heap, heap->size);
    heap->size++;
}

HeapNode extractMin(MinHeap* heap) {
    HeapNode empty = {-1, INT_MAX};
    if (heap->size <= 0) return empty;
    HeapNode min = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    return min;
}

void dijkstra(Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->numNodes) return;
    
    int dist[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    MinHeap* heap = createMinHeap(graph->numNodes);
    if (!heap) return;
    insertHeap(heap, start, 0);
    
    while (heap->size > 0) {
        HeapNode minNode = extractMin(heap);
        int u = minNode.node;
        
        for (int i = 0; i < graph->array[u].size; i++) {
            int v = graph->array[u].edges[i].dest;
            int weight = graph->array[u].edges[i].weight;
            if (weight < 0) continue;
            if (dist[u