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
    HeapNode* arr;
    int size;
    int capacity;
} MinHeap;

void initAdjList(AdjList* list) {
    list->capacity = 4;
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

void freeAdjList(AdjList* list) {
    free(list->edges);
}

void initHeap(MinHeap* heap, int capacity) {
    heap->capacity = capacity;
    heap->arr = malloc(heap->capacity * sizeof(HeapNode));
    heap->size = 0;
}

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int idx) {
    int parent = (idx - 1) / 2;
    if (idx > 0 && heap->arr[parent].dist > heap->arr[idx].dist) {
        swapHeapNodes(&heap->arr[parent], &heap->arr[idx]);
        heapifyUp(heap, parent);
    }
}

void heapifyDown(MinHeap* heap, int idx) {
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    int smallest = idx;
    if (left < heap->size && heap->arr[left].dist < heap->arr[smallest].dist) {
        smallest = left;
    }
    if (right < heap->size && heap->arr[right].dist < heap->arr[smallest].dist) {
        smallest = right;
    }
    if (smallest != idx) {
        swapHeapNodes(&heap->arr[idx], &heap->arr[smallest]);
        heapifyDown(heap, smallest);
    }
}

void heapPush(MinHeap* heap, int node, int dist) {
    if (heap->size >= heap->capacity) return;
    heap->arr[heap->size].node = node;
    heap->arr[heap->size].dist = dist;
    heapifyUp(heap, heap->size);
    heap->size++;
}

HeapNode heapPop(MinHeap* heap) {
    HeapNode empty = {-1, -1};
    if (heap->size <= 0) return empty;
    HeapNode root = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    return root;
}

void freeHeap(MinHeap* heap) {
    free(heap->arr);
}

void dijkstra(AdjList graph[], int n, int start, int distances[]) {
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < n; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    MinHeap heap;
    initHeap(&heap, n * 2);
    heapPush(&heap, start, 0);
    while (heap.size > 0) {
        HeapNode current = heapPop(&heap);
        int u = current.node;
        if (visited[u]) continue;
        visited[u] = 1;
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].edges[i].dest;
            int weight = graph[u].edges[i].weight;
            if (!visited[v] && distances[u] != INT_MAX) {
                int newDist = distances[u] + weight;
                if (newDist < distances[v]) {
                    distances[v] = newDist;
                    heapPush(&heap, v, newDist);
                }
            }
        }
    }
    freeHeap(&heap);
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
            for (int j = 0; j < n; j++) freeAdjList(&graph[j]);