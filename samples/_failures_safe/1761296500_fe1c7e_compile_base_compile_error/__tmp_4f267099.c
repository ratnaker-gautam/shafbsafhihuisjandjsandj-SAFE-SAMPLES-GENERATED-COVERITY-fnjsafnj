//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int node;
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
    list->size = 0;
    list->edges = malloc(list->capacity * sizeof(Edge));
}

void addEdge(AdjList* list, int node, int weight) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->edges = realloc(list->edges, list->capacity * sizeof(Edge));
    }
    list->edges[list->size].node = node;
    list->edges[list->size].weight = weight;
    list->size++;
}

void initHeap(MinHeap* heap) {
    heap->capacity = 20;
    heap->size = 0;
    heap->nodes = malloc(heap->capacity * sizeof(HeapNode));
}

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int idx) {
    if (idx == 0) return;
    int parent = (idx - 1) / 2;
    if (heap->nodes[parent].dist > heap->nodes[idx].dist) {
        swapHeapNodes(&heap->nodes[parent], &heap->nodes[idx]);
        heapifyUp(heap, parent);
    }
}

void heapifyDown(MinHeap* heap, int idx) {
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    int smallest = idx;
    
    if (left < heap->size && heap->nodes[left].dist < heap->nodes[smallest].dist) {
        smallest = left;
    }
    if (right < heap->size && heap->nodes[right].dist < heap->nodes[smallest].dist) {
        smallest = right;
    }
    if (smallest != idx) {
        swapHeapNodes(&heap->nodes[idx], &heap->nodes[smallest]);
        heapifyDown(heap, smallest);
    }
}

void heapPush(MinHeap* heap, int node, int dist) {
    if (heap->size >= heap->capacity) {
        heap->capacity *= 2;
        heap->nodes = realloc(heap->nodes, heap->capacity * sizeof(HeapNode));
    }
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].dist = dist;
    heap->size++;
    heapifyUp(heap, heap->size - 1);
}

HeapNode heapPop(MinHeap* heap) {
    HeapNode result = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    return result;
}

int isEmpty(MinHeap* heap) {
    return heap->size == 0;
}

void dijkstra(AdjList graph[], int start, int n, int dist[]) {
    MinHeap heap;
    initHeap(&heap);
    
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    heapPush(&heap, start, 0);
    
    while (!isEmpty(&heap)) {
        HeapNode current = heapPop(&heap);
        int u = current.node;
        
        if (current.dist != dist[u]) continue;
        
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].edges[i].node;
            int weight = graph[u].edges[i].weight;
            
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                heapPush(&heap, v, dist[v]);
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
    
    printf("Enter edges (from to weight):\n");
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
    printf("Enter start node: ");
    if (scanf("%d", &start) != 1