//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct Node {
    int id;
    int weight;
    struct Node* next;
} Node;

typedef struct Graph {
    Node* adjList[MAX_NODES];
    int numNodes;
} Graph;

typedef struct HeapNode {
    int node;
    int distance;
} HeapNode;

typedef struct MinHeap {
    HeapNode* array;
    int size;
    int capacity;
    int* positions;
} MinHeap;

Graph* createGraph(int numNodes) {
    if (numNodes <= 0 || numNodes > MAX_NODES) return NULL;
    Graph* graph = malloc(sizeof(Graph));
    if (!graph) return NULL;
    graph->numNodes = numNodes;
    for (int i = 0; i < numNodes; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) return;
    
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) return;
    newNode->id = dest;
    newNode->weight = weight;
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;
}

MinHeap* createMinHeap(int capacity) {
    if (capacity <= 0 || capacity > MAX_NODES) return NULL;
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->positions = malloc(capacity * sizeof(int));
    if (!heap->positions) {
        free(heap);
        return NULL;
    }
    heap->array = malloc(capacity * sizeof(HeapNode));
    if (!heap->array) {
        free(heap->positions);
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    for (int i = 0; i < capacity; i++) {
        heap->positions[i] = -1;
    }
    return heap;
}

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < heap->size && heap->array[left].distance < heap->array[smallest].distance) {
        smallest = left;
    }
    if (right < heap->size && heap->array[right].distance < heap->array[smallest].distance) {
        smallest = right;
    }
    if (smallest != idx) {
        heap->positions[heap->array[smallest].node] = idx;
        heap->positions[heap->array[idx].node] = smallest;
        swapHeapNodes(&heap->array[smallest], &heap->array[idx]);
        minHeapify(heap, smallest);
    }
}

HeapNode extractMin(MinHeap* heap) {
    if (heap->size <= 0) {
        HeapNode empty = {-1, INT_MAX};
        return empty;
    }
    HeapNode root = heap->array[0];
    heap->positions[root.node] = -1;
    if (heap->size > 1) {
        heap->array[0] = heap->array[heap->size - 1];
        heap->positions[heap->array[0].node] = 0;
    }
    heap->size--;
    minHeapify(heap, 0);
    return root;
}

void decreaseKey(MinHeap* heap, int node, int distance) {
    int i = heap->positions[node];
    if (i == -1 || heap->array[i].distance <= distance) return;
    
    heap->array[i].distance = distance;
    while (i > 0 && heap->array[(i - 1) / 2].distance > heap->array[i].distance) {
        heap->positions[heap->array[i].node] = (i - 1) / 2;
        heap->positions[heap->array[(i - 1) / 2].node] = i;
        swapHeapNodes(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void dijkstra(Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->numNodes) return;
    
    int distances[MAX_NODES];
    MinHeap* heap = createMinHeap(graph->numNodes);
    if (!heap) return;
    
    for (int i = 0; i < graph->numNodes; i++) {
        distances[i] = INT_MAX;
        heap->array[i].node = i;
        heap->array[i].distance = INT_MAX;
        heap->positions[i] = i;
    }
    
    heap->size = graph->numNodes;
    distances[start] = 0;
    decreaseKey(heap, start, 0);
    
    while (heap->size > 0) {
        HeapNode minNode = extractMin(heap);
        int u = minNode.node;
        
        if (minNode.distance == INT_MAX) break;
        
        Node