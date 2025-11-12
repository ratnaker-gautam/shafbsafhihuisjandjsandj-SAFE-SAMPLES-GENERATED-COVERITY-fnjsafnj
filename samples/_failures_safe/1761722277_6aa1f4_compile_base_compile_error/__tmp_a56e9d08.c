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
    int distance;
} HeapNode;

typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

Graph* createGraph(int numNodes) {
    if (numNodes <= 0 || numNodes > MAX_NODES) return NULL;
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;
    graph->numNodes = numNodes;
    graph->array = (AdjList*)malloc(numNodes * sizeof(AdjList));
    if (!graph->array) {
        free(graph);
        return NULL;
    }
    for (int i = 0; i < numNodes; i++) {
        graph->array[i].edges = (Edge*)malloc(4 * sizeof(Edge));
        if (!graph->array[i].edges) {
            for (int j = 0; j < i; j++) {
                free(graph->array[j].edges);
            }
            free(graph->array);
            free(graph);
            return NULL;
        }
        graph->array[i].size = 0;
        graph->array[i].capacity = 4;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes || weight < 0) return;
    
    AdjList* list = &graph->array[src];
    if (list->size >= list->capacity) {
        int new_capacity = list->capacity * 2;
        if (new_capacity <= list->capacity) return;
        Edge* new_edges = (Edge*)realloc(list->edges, new_capacity * sizeof(Edge));
        if (!new_edges) return;
        list->edges = new_edges;
        list->capacity = new_capacity;
    }
    list->edges[list->size].dest = dest;
    list->edges[list->size].weight = weight;
    list->size++;
}

MinHeap* createMinHeap(int capacity) {
    if (capacity <= 0) return NULL;
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->data = (HeapNode*)malloc(capacity * sizeof(HeapNode));
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

void minHeapify(MinHeap* heap, int idx) {
    if (!heap || idx < 0 || idx >= heap->size) return;
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->data[left].distance < heap->data[smallest].distance)
        smallest = left;
    if (right < heap->size && heap->data[right].distance < heap->data[smallest].distance)
        smallest = right;

    if (smallest != idx) {
        swapHeapNodes(&heap->data[idx], &heap->data[smallest]);
        minHeapify(heap, smallest);
    }
}

HeapNode extractMin(MinHeap* heap) {
    HeapNode empty = {-1, INT_MAX};
    if (!heap || heap->size <= 0) return empty;
    if (heap->size == 1) {
        heap->size--;
        return heap->data[0];
    }

    HeapNode root = heap->data[0];
    heap->data[0] = heap->data[heap->size-1];
    heap->size--;
    minHeapify(heap, 0);
    return root;
}

void decreaseKey(MinHeap* heap, int node, int distance) {
    if (!heap || node < 0) return;
    int i;
    for (i = 0; i < heap->size; i++) {
        if (heap->data[i].node == node) {
            heap->data[i].distance = distance;
            break;
        }
    }
    if (i >= heap->size) return;

    while (i != 0 && heap->data[(i-1)/2].distance > heap->data[i].distance) {
        swapHeapNodes(&heap->data[i], &heap->data[(i-1)/2]);
        i = (i-1)/2;
    }
}

int isInHeap(MinHeap* heap, int node) {
    if (!heap) return 0;
    for (int i = 0; i < heap->size; i++) {
        if (heap->data[i].node == node) return 1;
    }
    return 0;
}

void dijkstra(Graph* graph, int src) {
    if (!graph || src < 0 || src >= graph->numNodes) return;
    int dist[MAX_NODES];
    MinHeap* heap = createMinHeap(graph->numNodes);