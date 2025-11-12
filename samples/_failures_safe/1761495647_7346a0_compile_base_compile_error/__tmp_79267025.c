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

void init_graph(Graph* g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        exit(1);
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->edges[i][j] = 0;
        }
    }
}

void add_edge(Graph* g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        return;
    }
    if (weight < 0) {
        return;
    }
    g->edges[from][to] = weight;
    g->edges[to][from] = weight;
}

void heap_init(MinHeap* h) {
    h->size = 0;
}

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_push(MinHeap* h, int node, int distance) {
    if (h->size >= MAX_NODES) {
        return;
    }
    h->nodes[h->size].node = node;
    h->nodes[h->size].distance = distance;
    int current = h->size;
    h->size++;
    
    while (current > 0) {
        int parent = (current - 1) / 2;
        if (h->nodes[current].distance >= h->nodes[parent].distance) {
            break;
        }
        heap_swap(&h->nodes[current], &h->nodes[parent]);
        current = parent;
    }
}

int heap_pop(MinHeap* h) {
    if (h->size <= 0) {
        return -1;
    }
    int result = h->nodes[0].node;
    h->size--;
    h->nodes[0] = h->nodes[h->size];
    
    int current = 0;
    while (1) {
        int left = 2 * current + 1;
        int right = 2 * current + 2;
        int smallest = current;
        
        if (left < h->size && h->nodes[left].distance < h->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < h->size && h->nodes[right].distance < h->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == current) {
            break;
        }
        heap_swap(&h->nodes[current], &h->nodes[smallest]);
        current = smallest;
    }
    return result;
}

int heap_empty(MinHeap* h) {
    return h->size == 0;
}

void dijkstra(Graph* g, int start, int distances[]) {
    if (start < 0 || start >= g->node_count) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->node_count; i++) {
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
        if (visited[current]) {
            continue;
        }
        visited[current] = 1;
        
        for (int neighbor = 0; neighbor < g->node_count; neighbor++) {
            if (g->edges[current][neighbor] > 0 && !visited[neighbor]) {
                int new_dist = distances[current] + g->edges[current][neighbor];
                if (new_dist < distances[neighbor] && new_dist >= 0) {
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
    
    init_graph(&graph, 6);
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 1);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 3, 5, 6);
    add_edge(&graph, 4, 5, 3);
    
    printf("Shortest distances from node 0:\n");
    dijkstra(&graph, 0, distances);
    
    for (int i = 0; i < graph.node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %