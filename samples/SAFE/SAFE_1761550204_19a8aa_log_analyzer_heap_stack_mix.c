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

void init_log_heap(LogHeap* heap, int capacity) {
    heap->entries = malloc(capacity * sizeof(LogEntry));
    if (heap->entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    heap->count = 0;
    heap->capacity = capacity;
}

void free_log_heap(LogHeap* heap) {
    free(heap->entries);
    heap->entries = NULL;
    heap->count = 0;
    heap->capacity = 0;
}

int add_log_entry(LogHeap* heap, const char* timestamp, const char* level, const char* message) {
    if (heap->count >= heap->capacity) {
        return 0;
    }
    if (timestamp == NULL || level == NULL || message == NULL) {
        return 0;
    }
    if (strlen(timestamp) >= 31 || strlen(level) >= 15 || strlen(message) >= 255) {
        return 0;
    }
    
    LogEntry* entry = &heap->entries[heap->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    heap->count++;
    return 1;
}

void analyze_logs(LogHeap* heap) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < heap->count; i++) {
        const LogEntry* entry = &heap->entries[i];
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", heap->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (heap->count > 0) {
        printf("Error rate: %.2f%%\n", (error_count * 100.0) / heap->count);
    }
}

int main() {
    LogHeap heap;
    init_log_heap(&heap, MAX_ENTRIES);
    
    char input_buffer[MAX_LINE_LENGTH];
    char timestamp[32];
    char level[16];
    char message[256];
    
    printf("Enter log entries (format: timestamp level message)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Type 'quit' to finish input\n");
    
    while (1) {
        printf("> ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        
        if (strcmp(input_buffer, "quit") == 0) {
            break;
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        int parsed = sscanf(input_buffer, "%31s %15s %255[^\n]", 
                           timestamp, level, message);
        
        if (parsed == 3) {
            if (!add_log_entry(&heap, timestamp, level, message)) {
                printf("Failed to add log entry (buffer full or invalid data)\n");
            }
        } else {
            printf("Invalid format. Use: timestamp level message\n");
        }
        
        if (heap.count >= heap.capacity) {
            printf("Log buffer full. Starting analysis...\n");
            break;
        }
    }
    
    if (heap.count > 0) {
        analyze_logs(&heap);
    } else {
        printf("No log entries to analyze.\n");
    }
    
    free_log_heap(&heap);
    return 0;
}