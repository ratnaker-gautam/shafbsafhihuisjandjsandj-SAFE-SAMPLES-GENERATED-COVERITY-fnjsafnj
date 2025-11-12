//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int edges[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef struct {
    int node;
    int distance;
} HeapNode;

typedef struct {
    HeapNode nodes[MAX_NODES];
    int size;
} MinHeap;

void init_graph(Graph* graph, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        exit(1);
    }
    graph->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            graph->edges[i][j] = 0;
        }
    }
}

void add_edge(Graph* graph, int from, int to, int weight) {
    if (from < 0 || from >= graph->node_count || to < 0 || to >= graph->node_count) {
        return;
    }
    if (weight < 0) {
        return;
    }
    graph->edges[from][to] = weight;
    graph->edges[to][from] = weight;
}

void heap_init(MinHeap* heap) {
    heap->size = 0;
}

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_push(MinHeap* heap, int node, int distance) {
    if (heap->size >= MAX_NODES) {
        return;
    }
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].distance = distance;
    int current = heap->size;
    heap->size++;
    
    while (current > 0) {
        int parent = (current - 1) / 2;
        if (heap->nodes[current].distance >= heap->nodes[parent].distance) {
            break;
        }
        heap_swap(&heap->nodes[current], &heap->nodes[parent]);
        current = parent;
    }
}

int heap_pop(MinHeap* heap) {
    if (heap->size <= 0) {
        return -1;
    }
    int result = heap->nodes[0].node;
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    
    int current = 0;
    while (1) {
        int left = 2 * current + 1;
        int right = 2 * current + 2;
        int smallest = current;
        
        if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == current) {
            break;
        }
        heap_swap(&heap->nodes[current], &heap->nodes[smallest]);
        current = smallest;
    }
    return result;
}

int heap_empty(MinHeap* heap) {
    return heap->size == 0;
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
    heap_init(&heap);
    heap_push(&heap, start, 0);
    
    while (!heap_empty(&heap)) {
        int current = heap_pop(&heap);
        if (current == -1) {
            break;
        }
        
        for (int neighbor = 0; neighbor < graph->node_count; neighbor++) {
            if (graph->edges[current][neighbor] > 0) {
                int new_dist = distances[current] + graph->edges[current][neighbor];
                if (new_dist >= 0 && new_dist < distances[neighbor]) {
                    distances[neighbor] = new_dist;
                    heap_push(&heap, neighbor, new_dist);
                }
            }
        }
    }
}

int main() {
    Graph graph;
    int distances[MAX_NODES];
    
    printf("Enter number of nodes (1-100): ");
    int node_count;
    if (scanf("%d", &node_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    
    init_graph(&graph, node_count);
    
    printf("Enter number of edges: ");
    int edge_count;
    if (scanf("%d", &edge_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (edge_count < 0 || edge_count > node_count * (node_count - 1) / 2) {
        printf("Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges (from to weight):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3