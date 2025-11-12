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
    int distance;
} HeapNode;

typedef struct {
    HeapNode* elements;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->elements = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    if (!heap->elements) {
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
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < heap->size && heap->elements[left].distance < heap->elements[smallest].distance)
        smallest = left;
    if (right < heap->size && heap->elements[right].distance < heap->elements[smallest].distance)
        smallest = right;
    if (smallest != idx) {
        swapHeapNodes(&heap->elements[idx], &heap->elements[smallest]);
        minHeapify(heap, smallest);
    }
}

HeapNode extractMin(MinHeap* heap) {
    if (heap->size <= 0) {
        HeapNode empty = {-1, INT_MAX};
        return empty;
    }
    if (heap->size == 1) {
        heap->size--;
        return heap->elements[0];
    }
    HeapNode root = heap->elements[0];
    heap->elements[0] = heap->elements[heap->size - 1];
    heap->size--;
    minHeapify(heap, 0);
    return root;
}

void decreaseKey(MinHeap* heap, int node, int distance) {
    int i;
    for (i = 0; i < heap->size; i++) {
        if (heap->elements[i].node == node) {
            heap->elements[i].distance = distance;
            while (i && heap->elements[i].distance < heap->elements[(i - 1) / 2].distance) {
                swapHeapNodes(&heap->elements[i], &heap->elements[(i - 1) / 2]);
                i = (i - 1) / 2;
            }
            break;
        }
    }
}

int isInHeap(MinHeap* heap, int node) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->elements[i].node == node)
            return 1;
    }
    return 0;
}

void freeMinHeap(MinHeap* heap) {
    if (heap) {
        free(heap->elements);
        free(heap);
    }
}

int validateGraph(int numNodes, int numEdges, Edge* edges) {
    if (numNodes <= 0 || numNodes > MAX_NODES) return 0;
    if (numEdges < 0 || numEdges > MAX_EDGES) return 0;
    for (int i = 0; i < numEdges; i++) {
        if (edges[i].u < 0 || edges[i].u >= numNodes) return 0;
        if (edges[i].v < 0 || edges[i].v >= numNodes) return 0;
        if (edges[i].weight < 0) return 0;
    }
    return 1;
}

void dijkstra(int numNodes, int numEdges, Edge* edges, int startNode) {
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < numNodes; i++)
        dist[i] = INT_MAX;
    dist[startNode] = 0;
    
    MinHeap* heap = createMinHeap(numNodes);
    if (!heap) return;
    
    for (int i = 0; i < numNodes; i++) {
        heap->elements[i].node = i;
        heap->elements[i].distance = dist[i];
    }
    heap->size = numNodes;
    
    for (int i = (numNodes - 1) / 2; i >= 0; i--)
        minHeapify(heap, i);
    
    while (heap->size > 0) {
        HeapNode minNode = extractMin(heap);
        int u = minNode.node;
        if (minNode.distance == INT_MAX) break;
        visited[u] = 1;
        
        for (int i = 0; i < numEdges; i++) {
            if (edges[i].u == u) {
                int v = edges[i].v;
                if (!visited[v] && dist[u] != INT_MAX) {
                    int newDist = dist[u] + edges[i].weight;
                    if (newDist >= 0 && newDist < dist[v]) {
                        dist[v] = newDist;
                        if (isInHeap(heap, v))
                            decreaseKey(heap, v, dist[v]);
                    }
                }
            }
        }