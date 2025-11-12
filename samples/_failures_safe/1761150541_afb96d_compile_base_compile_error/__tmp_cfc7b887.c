//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct Node {
    int vertex;
    int weight;
    struct Node* next;
} Node;

typedef struct Graph {
    int numVertices;
    Node** adjLists;
} Graph;

typedef struct HeapNode {
    int vertex;
    int distance;
} HeapNode;

typedef struct MinHeap {
    int size;
    int capacity;
    int* positions;
    HeapNode** array;
} MinHeap;

Graph* createGraph(int vertices) {
    if (vertices <= 0 || vertices > MAX_NODES) return NULL;
    Graph* graph = malloc(sizeof(Graph));
    if (!graph) return NULL;
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(Node*));
    if (!graph->adjLists) {
        free(graph);
        return NULL;
    }
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

Node* createNode(int v, int weight) {
    if (weight < 0) return NULL;
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) return NULL;
    newNode->vertex = v;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

void addEdge(Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->numVertices || dest < 0 || dest >= graph->numVertices || weight < 0) return;
    Node* newNode = createNode(dest, weight);
    if (!newNode) return;
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    newNode = createNode(src, weight);
    if (!newNode) return;
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

MinHeap* createMinHeap(int capacity) {
    if (capacity <= 0 || capacity > MAX_NODES) return NULL;
    MinHeap* minHeap = malloc(sizeof(MinHeap));
    if (!minHeap) return NULL;
    minHeap->positions = malloc(capacity * sizeof(int));
    if (!minHeap->positions) {
        free(minHeap);
        return NULL;
    }
    minHeap->array = malloc(capacity * sizeof(HeapNode*));
    if (!minHeap->array) {
        free(minHeap->positions);
        free(minHeap);
        return NULL;
    }
    minHeap->size = 0;
    minHeap->capacity = capacity;
    for (int i = 0; i < capacity; i++) {
        minHeap->positions[i] = -1;
        minHeap->array[i] = NULL;
    }
    return minHeap;
}

HeapNode* newHeapNode(int v, int dist) {
    HeapNode* heapNode = malloc(sizeof(HeapNode));
    if (!heapNode) return NULL;
    heapNode->vertex = v;
    heapNode->distance = dist;
    return heapNode;
}

void swapHeapNodes(HeapNode** a, HeapNode** b) {
    HeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    if (!minHeap || idx < 0 || idx >= minHeap->size) return;
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]->distance < minHeap->array[smallest]->distance) smallest = left;
    if (right < minHeap->size && minHeap->array[right]->distance < minHeap->array[smallest]->distance) smallest = right;
    if (smallest != idx) {
        minHeap->positions[minHeap->array[smallest]->vertex] = idx;
        minHeap->positions[minHeap->array[idx]->vertex] = smallest;
        swapHeapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isEmpty(MinHeap* minHeap) {
    return !minHeap || minHeap->size == 0;
}

HeapNode* extractMin(MinHeap* minHeap) {
    if (isEmpty(minHeap)) return NULL;
    HeapNode* root = minHeap->array[0];
    HeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    minHeap->positions[root->vertex] = -1;
    minHeap->positions[lastNode->vertex] = 0;
    minHeap->size--;
    minHeapify(minHeap, 0);
    return root;
}

void decreaseKey(MinHeap* minHeap, int v, int dist) {
    if (!minHeap || v < 0 || v >= minHeap->capacity) return;
    int i = minHeap->positions[v];
    if (i < 0 || i >= minHeap->size) return;
    minHeap->array[i]->distance = dist;
    while (i && minHeap->array[i]->distance < minHeap->array[(i - 1) / 2]->distance) {
        minHeap->positions[minHeap->array[i]->vertex] = (i - 1) / 2;
        minHeap->positions[minHeap->array