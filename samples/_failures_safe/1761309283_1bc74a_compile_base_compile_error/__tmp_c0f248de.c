//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adj_matrix[MAX_NODES][MAX_NODES];
    char node_names[MAX_NODES][32];
};

struct Queue {
    int items[MAX_NODES];
    int front;
    int rear;
};

void init_queue(struct Queue* q) {
    q->front = -1;
    q->rear = -1;
}

int is_empty(struct Queue* q) {
    return q->front == -1;
}

void enqueue(struct Queue* q, int value) {
    if (q->rear == MAX_NODES - 1) {
        return;
    }
    if (q->front == -1) {
        q->front = 0;
    }
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(struct Queue* q) {
    if (is_empty(q)) {
        return -1;
    }
    int item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        q->front = q->rear = -1;
    }
    return item;
}

void init_graph(struct Graph* g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adj_matrix[i][j] = 0;
        }
        g->node_names[i][0] = '\0';
    }
}

int find_node_index(struct Graph* g, const char* name) {
    for (int i = 0; i < g->num_nodes; i++) {
        if (strcmp(g->node_names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

int add_node(struct Graph* g, const char* name) {
    if (g->num_nodes >= MAX_NODES) {
        return -1;
    }
    if (find_node_index(g, name) != -1) {
        return -1;
    }
    strncpy(g->node_names[g->num_nodes], name, 31);
    g->node_names[g->num_nodes][31] = '\0';
    g->num_nodes++;
    return g->num_nodes - 1;
}

int add_edge(struct Graph* g, const char* from, const char* to, int weight) {
    if (weight <= 0) {
        return -1;
    }
    int from_idx = find_node_index(g, from);
    int to_idx = find_node_index(g, to);
    if (from_idx == -1 || to_idx == -1) {
        return -1;
    }
    g->adj_matrix[from_idx][to_idx] = weight;
    g->num_edges++;
    return 0;
}

void bfs(struct Graph* g, int start_node) {
    if (start_node < 0 || start_node >= g->num_nodes) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    struct Queue q;
    init_queue(&q);
    
    visited[start_node] = 1;
    enqueue(&q, start_node);
    
    printf("BFS traversal: ");
    while (!is_empty(&q)) {
        int current = dequeue(&q);
        printf("%s ", g->node_names[current]);
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adj_matrix[current][i] > 0 && !visited[i]) {
                visited[i] = 1;
                enqueue(&q, i);
            }
        }
    }
    printf("\n");
}

void dfs_util(struct Graph* g, int node, int visited[]) {
    visited[node] = 1;
    printf("%s ", g->node_names[node]);
    
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[node][i] > 0 && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph* g, int start_node) {
    if (start_node < 0 || start_node >= g->num_nodes) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_util(g, start_node, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf("Graph Traversal Program\n");
    printf("Enter nodes and edges to build a graph\n");
    
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    int num_nodes;
    if (scanf("%d", &num_nodes) != 1 || num_nodes <= 0 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    for (int i = 0; i < num_nodes; i++) {
        char name[32];
        printf("Enter node %d name: ", i + 1);
        if (scanf("%31s", name) != 1) {
            printf("Invalid node name\n");
            return 1;
        }
        if (add_node(&g, name) ==