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
    int node;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* nodes;
    int size;
    int capacity;
} MinHeap;

void initAdjList(AdjList* list) {
    list->capacity = 10;
    list->edges = malloc(list->capacity * sizeof(Edge));
    list->size = 0;
}

void addEdge(AdjList* list, int dest, int weight) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->edges = realloc(list->edges, list->capacity * sizeof(Edge));
    }
    list->edges[list->size].dest = dest;
    list->edges[list->size].weight = weight;
    list->size++;
}

void initHeap(MinHeap* heap, int capacity) {
    heap->capacity = capacity;
    heap->nodes = malloc(heap->capacity * sizeof(HeapNode));
    heap->size = 0;
}

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->nodes[parent].dist <= heap->nodes[idx].dist) break;
        swapHeapNodes(&heap->nodes[parent], &heap->nodes[idx]);
        idx = parent;
    }
}

void heapifyDown(MinHeap* heap, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < heap->size && heap->nodes[left].dist < heap->nodes[smallest].dist)
            smallest = left;
        if (right < heap->size && heap->nodes[right].dist < heap->nodes[smallest].dist)
            smallest = right;
        
        if (smallest == idx) break;
        swapHeapNodes(&heap->nodes[idx], &heap->nodes[smallest]);
        idx = smallest;
    }
}

void heapPush(MinHeap* heap, int node, int dist) {
    if (heap->size >= heap->capacity) return;
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].dist = dist;
    heapifyUp(heap, heap->size);
    heap->size++;
}

HeapNode heapPop(MinHeap* heap) {
    HeapNode result = heap->nodes[0];
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    heapifyDown(heap, 0);
    return result;
}

int isEmpty(MinHeap* heap) {
    return heap->size == 0;
}

void dijkstra(AdjList graph[], int n, int start, int distances[]) {
    MinHeap heap;
    initHeap(&heap, n);
    
    for (int i = 0; i < n; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    heapPush(&heap, start, 0);
    
    while (!isEmpty(&heap)) {
        HeapNode current = heapPop(&heap);
        int u = current.node;
        
        if (current.dist != distances[u]) continue;
        
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].edges[i].dest;
            int weight = graph[u].edges[i].weight;
            
            if (weight < 0) continue;
            
            if (distances[u] != INT_MAX) {
                int new_dist = distances[u] + weight;
                if (new_dist < distances[v]) {
                    distances[v] = new_dist;
                    heapPush(&heap, v, new_dist);
                }
            }
        }
    }
    
    free(heap.nodes);
}

int main() {
    int n, m;
    printf("Enter number of nodes and edges: ");
    if (scanf("%d %d", &n, &m) != 2 || n <= 0 || n > MAX_NODES || m < 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    AdjList graph[MAX_NODES];
    for (int i = 0; i < n; i++) {
        initAdjList(&graph[i]);
    }
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3 || u < 0 || u >= n || v < 0 || v >= n || w < 0) {
            printf("Invalid edge\n");
            return 1;
        }
        addEdge(&graph[u], v, w);
        addEdge(&graph[v], u, w);
    }
    
    int start;
    printf("Enter starting node: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= n) {
        printf("Invalid