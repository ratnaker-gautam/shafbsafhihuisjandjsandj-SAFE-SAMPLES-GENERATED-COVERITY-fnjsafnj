//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int matrix[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(EXIT_FAILURE);
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->matrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* g, int from, int to, int weight) {
    if (from < 0 || from >= g->nodes || to < 0 || to >= g->nodes) {
        fprintf(stderr, "Invalid edge nodes\n");
        exit(EXIT_FAILURE);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weight not allowed\n");
        exit(EXIT_FAILURE);
    }
    g->matrix[from][to] = weight;
    g->matrix[to][from] = weight;
}

void dfsUtil(struct Graph* g, int node, int* visited, void (*visit)(int)) {
    if (node < 0 || node >= g->nodes) return;
    visited[node] = 1;
    visit(node);
    for (int i = 0; i < g->nodes; i++) {
        if (g->matrix[node][i] != 0 && !visited[i]) {
            dfsUtil(g, i, visited, visit);
        }
    }
}

void dfs(struct Graph* g, int start, void (*visit)(int)) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    dfsUtil(g, start, visited, visit);
}

void bfs(struct Graph* g, int start, void (*visit)(int)) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        visit(current);
        for (int i = 0; i < g->nodes; i++) {
            if (g->matrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
                queue[rear++] = i;
            }
        }
    }
}

void printNode(int node) {
    printf("%d ", node);
}

void processNode(int node) {
    printf("Processing node %d\n", node);
}

int main() {
    struct Graph g;
    initializeGraph(&g, 6);
    addEdge(&g, 0, 1, 2);
    addEdge(&g, 0, 2, 4);
    addEdge(&g, 1, 2, 1);
    addEdge(&g, 1, 3, 7);
    addEdge(&g, 2, 4, 3);
    addEdge(&g, 3, 4, 1);
    addEdge(&g, 3, 5, 5);
    addEdge(&g, 4, 5, 2);
    
    printf("DFS traversal: ");
    dfs(&g, 0, printNode);
    printf("\n");
    
    printf("BFS traversal: ");
    bfs(&g, 0, printNode);
    printf("\n");
    
    printf("Processing nodes with DFS:\n");
    dfs(&g, 0, processNode);
    
    return 0;
}