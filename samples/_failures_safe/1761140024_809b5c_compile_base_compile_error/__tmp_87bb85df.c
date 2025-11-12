//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t count;
    size_t capacity;
} LogHeap;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    size_t count;
} LogStack;

void heap_init(LogHeap *heap) {
    heap->capacity = 16;
    heap->count = 0;
    heap->entries = malloc(heap->capacity * sizeof(LogEntry));
    if (!heap->entries) exit(1);
}

void heap_add(LogHeap *heap, const char *timestamp, const char *level, const char *message) {
    if (heap->count >= heap->capacity) {
        size_t new_cap = heap->capacity * 2;
        if (new_cap > MAX_ENTRIES) new_cap = MAX_ENTRIES;
        LogEntry *new_entries = realloc(heap->entries, new_cap * sizeof(LogEntry));
        if (!new_entries) return;
        heap->entries = new_entries;
        heap->capacity = new_cap;
    }
    if (heap->count >= MAX_ENTRIES) return;
    
    LogEntry *entry = &heap->entries[heap->count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp)-1);
    entry->timestamp[sizeof(entry->timestamp)-1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level)-1);
    entry->level[sizeof(entry->level)-1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message)-1);
    entry->message[sizeof(entry->message)-1] = '\0';
    heap->count++;
}

void stack_init(LogStack *stack) {
    stack->count = 0;
}

int stack_add(LogStack *stack, const char *timestamp, const char *level, const char *message) {
    if (stack->count >= MAX_ENTRIES) return 0;
    
    LogEntry *entry = &stack->entries[stack->count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp)-1);
    entry->timestamp[sizeof(entry->timestamp)-1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level)-1);
    entry->level[sizeof(entry->level)-1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message)-1);
    entry->message[sizeof(entry->message)-1] = '\0';
    stack->count++;
    return 1;
}

void analyze_logs(LogHeap *heap, LogStack *stack) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < heap->count; i++) {
        const LogEntry *entry = &heap->entries[i];
        if (strcmp(entry->level, "ERROR") == 0) error_count++;
        else if (strcmp(entry->level, "WARNING") == 0) warning_count++;
        else if (strcmp(entry->level, "INFO") == 0) info_count++;
    }
    
    for (size_t i = 0; i < stack->count; i++) {
        const LogEntry *entry = &stack->entries[i];
        if (strcmp(entry->level, "ERROR") == 0) error_count++;
        else if (strcmp(entry->level, "WARNING") == 0) warning_count++;
        else if (strcmp(entry->level, "INFO") == 0) info_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %zu\n", heap->count + stack->count);
}

void cleanup(LogHeap *heap) {
    free(heap->entries);
}

int main() {
    LogHeap heap_logs;
    LogStack stack_logs;
    
    heap_init(&heap_logs);
    stack_init(&stack_logs);
    
    printf("Enter log entries (format: timestamp level message)\n");
    printf("Type 'done' to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    int use_heap = 1;
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "done", 4) == 0) break;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) == 3) {
            if (use_heap) {
                heap_add(&heap_logs, timestamp, level, message);
            } else {
                if (!stack_add(&stack_logs, timestamp, level, message)) {
                    printf("Stack full, switching to heap\n");
                    heap_add(&heap_logs, timestamp, level, message);