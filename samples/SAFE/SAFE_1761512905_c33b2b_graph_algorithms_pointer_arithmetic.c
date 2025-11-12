//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

struct Node {
    int id;
    int weight;
    struct Node* next;
};

struct Graph {
    int num_nodes;
    struct Node* adjacency[MAX_NODES];
};

void init_graph(struct Graph* g, int n) {
    if (n <= 0 || n > MAX_NODES) {
        n = MAX_NODES;
    }
    g->num_nodes = n;
    for (int i = 0; i < n; i++) {
        *(g->adjacency + i) = NULL;
    }
}

int add_edge(struct Graph* g, int src, int dest, int weight) {
    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    
    struct Node* new_node = malloc(sizeof(struct Node));
    if (!new_node) {
        return 0;
    }
    new_node->id = dest;
    new_node->weight = weight;
    new_node->next = *(g->adjacency + src);
    *(g->adjacency + src) = new_node;
    return 1;
}

void dfs_util(struct Graph* g, int node, int* visited) {
    if (node < 0 || node >= g->num_nodes) {
        return;
    }
    if (*(visited + node)) {
        return;
    }
    
    printf("%d ", node);
    *(visited + node) = 1;
    
    struct Node* current = *(g->adjacency + node);
    while (current != NULL) {
        dfs_util(g, current->id, visited);
        current = current->next;
    }
}

void dfs(struct Graph* g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        printf("Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(visited + i) = 0;
    }
    
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void bfs(struct Graph* g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        printf("Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(visited + i) = 0;
    }
    
    int queue[MAX_NODES];
    int front = 0;
    int rear = 0;
    
    *(visited + start) = 1;
    *(queue + rear) = start;
    rear = (rear + 1) % MAX_NODES;
    
    printf("BFS traversal: ");
    
    while (front != rear) {
        int current = *(queue + front);
        front = (front + 1) % MAX_NODES;
        printf("%d ", current);
        
        struct Node* temp = *(g->adjacency + current);
        while (temp != NULL) {
            int neighbor = temp->id;
            if (!*(visited + neighbor)) {
                *(visited + neighbor) = 1;
                *(queue + rear) = neighbor;
                rear = (rear + 1) % MAX_NODES;
            }
            temp = temp->next;
        }
    }
    printf("\n");
}

void free_graph(struct Graph* g) {
    for (int i = 0; i < g->num_nodes; i++) {
        struct Node* current = *(g->adjacency + i);
        while (current != NULL) {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
}

int main() {
    struct Graph g;
    int num_nodes = 6;
    
    init_graph(&g, num_nodes);
    
    add_edge(&g, 0, 1, 1);
    add_edge(&g, 0, 2, 1);
    add_edge(&g, 1, 3, 1);
    add_edge(&g, 1, 4, 1);
    add_edge(&g, 2, 4, 1);
    add_edge(&g, 3, 5, 1);
    add_edge(&g, 4, 5, 1);
    
    printf("Graph with %d nodes\n", num_nodes);
    dfs(&g, 0);
    bfs(&g, 0);
    
    free_graph(&g);
    return 0;
}