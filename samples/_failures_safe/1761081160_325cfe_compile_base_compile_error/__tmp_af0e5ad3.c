//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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
    if (weight < 0 || weight > 100) {
        return -1;
    }
    int from_idx = find_node_index(g, from);
    int to_idx = find_node_index(g, to);
    if (from_idx == -1 || to_idx == -1) {
        return -1;
    }
    if (g->adj_matrix[from_idx][to_idx] != 0) {
        return -1;
    }
    g->adj_matrix[from_idx][to_idx] = weight;
    g->num_edges++;
    return 0;
}

void bfs(struct Graph* g, const char* start_name) {
    int start_idx = find_node_index(g, start_name);
    if (start_idx == -1) {
        printf("Start node not found\n");
        return;
    }

    int visited[MAX_NODES] = {0};
    struct Queue q;
    init_queue(&q);

    visited[start_idx] = 1;
    enqueue(&q, start_idx);

    printf("BFS traversal from %s: ", start_name);

    while (!is_empty(&q)) {
        int current = dequeue(&q);
        printf("%s ", g->node_names[current]);

        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adj_matrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                enqueue(&q, i);
            }
        }
    }
    printf("\n");
}

void dfs_util(struct Graph* g, int node, int* visited) {
    visited[node] = 1;
    printf("%s ", g->node_names[node]);

    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adj_matrix[node][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph* g, const char* start_name) {
    int start_idx = find_node_index(g, start_name);
    if (start_idx == -1) {
        printf("Start node not found\n");
        return;
    }

    int visited[MAX_NODES] = {0};
    printf("DFS traversal from %s: ", start_name);
    dfs_util(g, start_idx, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    init_graph(&g);

    printf("Graph Algorithms Demo\n");
    printf("Enter nodes and edges to build a graph\n");

    char input[64];
    int choice = 0;

    while (choice != 5) {
        printf("\n1. Add node\n2. Add edge\n3. BFS\n4. DFS\n5. Exit\n");
        printf("Enter choice: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if