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
    LogEntry* entries;
    int count;
    int capacity;
} LogHeap;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    int count;
} LogStack;

void init_heap_log(LogHeap* heap, int capacity) {
    if (capacity <= 0 || capacity > 10000) capacity = 100;
    heap->entries = malloc(capacity * sizeof(LogEntry));
    if (heap->entries == NULL) exit(1);
    heap->count = 0;
    heap->capacity = capacity;
}

void free_heap_log(LogHeap* heap) {
    if (heap->entries != NULL) {
        free(heap->entries);
        heap->entries = NULL;
    }
    heap->count = 0;
    heap->capacity = 0;
}

int add_to_heap_log(LogHeap* heap, const char* timestamp, const char* level, const char* message) {
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
    
    heap->count++;
    return 1;
}

void init_stack_log(LogStack* stack) {
    stack->count = 0;
}

int add_to_stack_log(LogStack* stack, const char* timestamp, const char* level, const char* message) {
    if (stack == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (stack->count >= MAX_ENTRIES) return 0;
    
    size_t ts_len = strlen(timestamp);
    size_t lvl_len = strlen(level);
    size_t msg_len = strlen(message);
    
    if (ts_len >= 32 || lvl_len >= 16 || msg_len >= 256) return 0;
    
    strncpy(stack->entries[stack->count].timestamp, timestamp, 31);
    stack->entries[stack->count].timestamp[31] = '\0';
    strncpy(stack->entries[stack->count].level, level, 15);
    stack->entries[stack->count].level[15] = '\0';
    strncpy(stack->entries[stack->count].message, message, 255);
    stack->entries[stack->count].message[255] = '\0';
    
    stack->count++;
    return 1;
}

void analyze_logs(LogHeap* heap_log, LogStack* stack_log) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < heap_log->count; i++) {
        if (strcmp(heap_log->entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(heap_log->entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(heap_log->entries[i].level, "INFO") == 0) info_count++;
    }
    
    for (int i = 0; i < stack_log->count; i++) {
        if (strcmp(stack_log->entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(stack_log->entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(stack_log->entries[i].level, "INFO") == 0) info_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", heap_log->count + stack_log->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
}

int main() {
    LogHeap heap_log;
    LogStack stack_log;
    
    init_heap_log(&heap_log, 50);
    init_stack_log(&stack_log);
    
    printf("Enter log entries (format: timestamp level message). Type 'done' to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "done", 4) == 0) break;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int result = sscanf(line, "%