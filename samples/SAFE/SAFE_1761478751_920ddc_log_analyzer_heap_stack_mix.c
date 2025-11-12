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
    int count;
    int capacity;
} LogHeap;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char level[16];
    char message[256];
    char timestamp[32];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void init_log_heap(LogHeap *heap, int initial_capacity) {
    if (!heap || initial_capacity <= 0) return;
    
    heap->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!heap->entries) return;
    
    heap->count = 0;
    heap->capacity = initial_capacity;
}

void add_to_heap(LogHeap *heap, const LogEntry *entry) {
    if (!heap || !entry || heap->count >= heap->capacity) return;
    
    if (heap->count < heap->capacity) {
        heap->entries[heap->count] = *entry;
        heap->count++;
    }
}

void analyze_logs(LogHeap *heap) {
    if (!heap || heap->count == 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < heap->count; i++) {
        if (strcmp(heap->entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(heap->entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(heap->entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", heap->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        for (int i = heap->count - 1; i >= 0 && i >= heap->count - 3; i--) {
            if (strcmp(heap->entries[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", heap->entries[i].timestamp, heap->entries[i].message);
            }
        }
    }
}

void cleanup_heap(LogHeap *heap) {
    if (heap && heap->entries) {
        free(heap->entries);
        heap->entries = NULL;
        heap->count = 0;
        heap->capacity = 0;
    }
}

int main() {
    LogHeap heap;
    init_log_heap(&heap, 100);
    
    if (!heap.entries) {
        fprintf(stderr, "Failed to initialize log heap\n");
        return 1;
    }
    
    char input_buffer[MAX_LINE_LENGTH];
    LogEntry stack_entry;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        if (strncmp(input_buffer, "END", 3) == 0) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (parse_log_line(input_buffer, &stack_entry)) {
            add_to_heap(&heap, &stack_entry);
        } else {
            printf("Invalid log format. Skipping: %s\n", input_buffer);
        }
        
        if (heap.count >= heap.capacity) {
            break;
        }
    }
    
    if (heap.count > 0) {
        analyze_logs(&heap);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    cleanup_heap(&heap);
    return 0;
}