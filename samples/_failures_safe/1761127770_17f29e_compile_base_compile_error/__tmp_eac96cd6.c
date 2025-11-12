//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

typedef struct {
    LogEntry *entries;
    int count;
    int capacity;
} LogHeap;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    int top;
} LogStack;

void init_heap(LogHeap *heap, int capacity) {
    if (capacity <= 0 || capacity > 10000) capacity = 100;
    heap->entries = malloc(capacity * sizeof(LogEntry));
    if (heap->entries == NULL) exit(1);
    heap->count = 0;
    heap->capacity = capacity;
}

void free_heap(LogHeap *heap) {
    if (heap->entries != NULL) {
        free(heap->entries);
        heap->entries = NULL;
    }
    heap->count = 0;
    heap->capacity = 0;
}

int add_to_heap(LogHeap *heap, const char *timestamp, const char *level, const char *message) {
    if (heap == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (heap->count >= heap->capacity) return 0;
    
    size_t ts_len = strlen(timestamp);
    size_t lvl_len = strlen(level);
    size_t msg_len = strlen(message);
    
    if (ts_len >= 32 || lvl_len >= 16 || msg_len >= 256) return 0;
    
    strncpy(heap->entries[heap->count].timestamp, timestamp, 31);
    heap->entries[heap->count].timestamp[31] = '\0';
    strncpy(heap->entries[heap->count].level, level, 15);
    heap->entries[heap->count].level[15] = '\0';
    strncpy(heap->entries[heap->count].message, message, 255);
    heap->entries[heap->count].message[255] = '\0';
    
    if (strcmp(level, "ERROR") == 0) heap->entries[heap->count].severity = 3;
    else if (strcmp(level, "WARN") == 0) heap->entries[heap->count].severity = 2;
    else if (strcmp(level, "INFO") == 0) heap->entries[heap->count].severity = 1;
    else heap->entries[heap->count].severity = 0;
    
    heap->count++;
    return 1;
}

void init_stack(LogStack *stack) {
    stack->top = -1;
}

int push_to_stack(LogStack *stack, const LogEntry *entry) {
    if (stack == NULL || entry == NULL) return 0;
    if (stack->top >= MAX_ENTRIES - 1) return 0;
    
    stack->top++;
    stack->entries[stack->top] = *entry;
    return 1;
}

int pop_from_stack(LogStack *stack, LogEntry *entry) {
    if (stack == NULL || entry == NULL) return 0;
    if (stack->top < 0) return 0;
    
    *entry = stack->entries[stack->top];
    stack->top--;
    return 1;
}

void analyze_logs(LogHeap *heap, LogStack *stack) {
    if (heap == NULL || stack == NULL) return;
    
    for (int i = 0; i < heap->count; i++) {
        if (heap->entries[i].severity >= 2) {
            if (!push_to_stack(stack, &heap->entries[i])) {
                break;
            }
        }
    }
}

void print_analysis(LogStack *stack) {
    if (stack == NULL) return;
    
    printf("High severity logs (WARN/ERROR):\n");
    printf("--------------------------------\n");
    
    LogStack temp_stack;
    init_stack(&temp_stack);
    
    while (stack->top >= 0) {
        LogEntry entry;
        if (pop_from_stack(stack, &entry)) {
            printf("Time: %s | Level: %s | Message: %s\n", 
                   entry.timestamp, entry.level, entry.message);
            push_to_stack(&temp_stack, &entry);
        } else {
            break;
        }
    }
    
    while (temp_stack.top >= 0) {
        LogEntry entry;
        if (pop_from_stack(&temp_stack, &entry)) {
            push_to_stack(stack, &entry);
        } else {
            break;
        }
    }
}

int main() {
    LogHeap heap;
    LogStack stack;
    
    init_heap(&heap, 50);
    init_stack(&stack);
    
    printf("Enter log entries (format: timestamp level message)\n");
    printf("Supported levels: INFO, WARN, ERROR\n");
    printf("Type 'done' to finish input\n");
    
    char line[MAX_LINE_LEN];
    int entry_count = 0;
    
    while (entry_count < 50) {
        printf("Log %d: ", entry_count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "done") == 0