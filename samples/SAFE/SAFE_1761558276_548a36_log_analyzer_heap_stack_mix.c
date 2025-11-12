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
    int severity;
} LogEntry;

typedef struct {
    LogEntry* entries;
    int count;
    int capacity;
} LogHeap;

void init_log_heap(LogHeap* heap, int capacity) {
    if (capacity <= 0 || capacity > 10000) capacity = 100;
    heap->entries = malloc(sizeof(LogEntry) * capacity);
    if (!heap->entries) exit(1);
    heap->count = 0;
    heap->capacity = capacity;
}

void free_log_heap(LogHeap* heap) {
    if (heap->entries) free(heap->entries);
    heap->entries = NULL;
    heap->count = 0;
    heap->capacity = 0;
}

int add_log_entry(LogHeap* heap, const char* timestamp, const char* level, const char* message) {
    if (!heap || !timestamp || !level || !message) return 0;
    if (heap->count >= heap->capacity) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    LogEntry* entry = &heap->entries[heap->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    if (strcmp(level, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(level, "WARN") == 0) entry->severity = 2;
    else if (strcmp(level, "INFO") == 0) entry->severity = 1;
    else entry->severity = 0;
    
    heap->count++;
    return 1;
}

void analyze_logs(LogHeap* heap) {
    if (!heap || heap->count == 0) return;
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int other_count = 0;
    
    for (int i = 0; i < heap->count; i++) {
        LogEntry* entry = &heap->entries[i];
        if (entry->severity == 3) error_count++;
        else if (entry->severity == 2) warn_count++;
        else if (entry->severity == 1) info_count++;
        else other_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", heap->count);
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    printf("OTHER: %d\n", other_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        int printed = 0;
        for (int i = heap->count - 1; i >= 0 && printed < 3; i--) {
            if (heap->entries[i].severity == 3) {
                printf("- %s: %s\n", heap->entries[i].timestamp, heap->entries[i].message);
                printed++;
            }
        }
    }
}

int main() {
    LogHeap heap;
    init_log_heap(&heap, 50);
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'quit' to finish input\n");
    
    while (entry_count < 50) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (!fgets(line, sizeof(line), stdin)) break;
        
        if (strncmp(line, "quit", 4) == 0) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed >= 2) {
            if (add_log_entry(&heap, timestamp, level, message)) {
                entry_count++;
            } else {
                printf("Failed to add log entry (invalid data or full)\n");
            }
        } else {
            printf("Invalid format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(&heap);
    } else {
        printf("No log entries to analyze.\n");
    }
    
    free_log_heap(&heap);
    return 0;
}