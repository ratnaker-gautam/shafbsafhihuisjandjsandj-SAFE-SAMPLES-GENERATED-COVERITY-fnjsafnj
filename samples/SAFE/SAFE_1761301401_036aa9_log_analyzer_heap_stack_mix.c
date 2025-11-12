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
    if (capacity <= 0 || capacity > 10000) capacity = 100;
    heap->entries = malloc(capacity * sizeof(LogEntry));
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
    
    if (strlen(timestamp) >= 31) return 0;
    if (strlen(level) >= 15) return 0;
    if (strlen(message) >= 255) return 0;
    
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
    if (!heap || heap->count == 0) return;
    
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
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        int printed = 0;
        for (int i = heap->count - 1; i >= 0 && printed < 3; i--) {
            const LogEntry* entry = &heap->entries[i];
            if (strcmp(entry->level, "ERROR") == 0) {
                printf("- %s: %s\n", entry->timestamp, entry->message);
                printed++;
            }
        }
    }
}

int main() {
    LogHeap heap;
    init_log_heap(&heap, 50);
    
    char input[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: timestamp level message)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Type 'analyze' to show results or 'quit' to exit\n\n");
    
    while (entry_count < heap.capacity) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (!fgets(input, sizeof(input), stdin)) break;
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) break;
        if (strcmp(input, "analyze") == 0) {
            analyze_logs(&heap);
            continue;
        }
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(input, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed == 3) {
            if (add_log_entry(&heap, timestamp, level, message)) {
                entry_count++;
                printf("Entry added successfully.\n");
            } else {
                printf("Failed to add entry.\n");
            }
        } else {
            printf("Invalid format. Use: timestamp level message\n");
        }
    }
    
    if (heap.count > 0) {
        printf("\nFinal analysis:\n");
        analyze_logs(&heap);
    }
    
    free_log_heap(&heap);
    return 0;
}