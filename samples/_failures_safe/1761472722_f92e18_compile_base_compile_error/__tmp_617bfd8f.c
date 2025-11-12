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

void init_log_heap(LogHeap *heap, size_t capacity) {
    if (capacity == 0 || capacity > MAX_ENTRIES) capacity = MAX_ENTRIES;
    heap->entries = malloc(capacity * sizeof(LogEntry));
    if (heap->entries == NULL) exit(EXIT_FAILURE);
    heap->count = 0;
    heap->capacity = capacity;
}

void free_log_heap(LogHeap *heap) {
    free(heap->entries);
    heap->entries = NULL;
    heap->count = 0;
    heap->capacity = 0;
}

int add_log_entry(LogHeap *heap, const char *timestamp, const char *level, const char *message) {
    if (heap->count >= heap->capacity) return 0;
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    size_t ts_len = strlen(timestamp);
    size_t lvl_len = strlen(level);
    size_t msg_len = strlen(message);
    
    if (ts_len >= sizeof(heap->entries[heap->count].timestamp) ||
        lvl_len >= sizeof(heap->entries[heap->count].level) ||
        msg_len >= sizeof(heap->entries[heap->count].message)) {
        return 0;
    }
    
    strncpy(heap->entries[heap->count].timestamp, timestamp, sizeof(heap->entries[heap->count].timestamp) - 1);
    strncpy(heap->entries[heap->count].level, level, sizeof(heap->entries[heap->count].level) - 1);
    strncpy(heap->entries[heap->count].message, message, sizeof(heap->entries[heap->count].message) - 1);
    
    heap->entries[heap->count].timestamp[sizeof(heap->entries[heap->count].timestamp) - 1] = '\0';
    heap->entries[heap->count].level[sizeof(heap->entries[heap->count].level) - 1] = '\0';
    heap->entries[heap->count].message[sizeof(heap->entries[heap->count].message) - 1] = '\0';
    
    heap->count++;
    return 1;
}

void analyze_logs(LogHeap *heap) {
    if (heap->count == 0) {
        printf("No logs to analyze.\n");
        return;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < heap->count; i++) {
        if (strcmp(heap->entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(heap->entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(heap->entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", heap->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        int printed = 0;
        for (size_t i = heap->count - 1; i < heap->count && printed < 3; i--) {
            if (strcmp(heap->entries[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", heap->entries[i].timestamp, heap->entries[i].message);
                printed++;
            }
        }
    }
}

int main(void) {
    LogHeap heap;
    init_log_heap(&heap, 50);
    
    char line[MAX_LINE_LENGTH];
    char timestamp[32];
    char level[16];
    char message[256];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR\n");
    printf("Enter 'quit' to finish input\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "quit", 4) == 0) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            printf("Invalid format. Use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        if (strcmp(level, "INFO") != 0 && 
            strcmp(level, "WARNING") != 0 && 
            strcmp(level, "ERROR") != 0) {
            printf("Invalid level. Use INFO, WARNING, or ERROR\n");
            continue;
        }