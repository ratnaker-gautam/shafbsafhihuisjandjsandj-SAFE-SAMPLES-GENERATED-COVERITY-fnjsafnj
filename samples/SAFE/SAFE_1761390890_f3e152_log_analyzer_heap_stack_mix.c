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
    
    if (ts_len >= 32 || lvl_len >= 16 || msg_len >= 256) return 0;
    
    LogEntry *entry = &heap->entries[heap->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
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
        const LogEntry *entry = &heap->entries[i];
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
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
        for (size_t i = heap->count; i > 0 && printed < 3; i--) {
            const LogEntry *entry = &heap->entries[i-1];
            if (strcmp(entry->level, "ERROR") == 0) {
                printf("- [%s] %s\n", entry->timestamp, entry->message);
                printed++;
            }
        }
    }
}

int main() {
    LogHeap heap;
    init_log_heap(&heap, 50);
    
    char input_buffer[MAX_LINE_LENGTH];
    char timestamp[32];
    char level[16];
    char message[256];
    
    printf("Log Analyzer - Enter log entries (format: timestamp level message)\n");
    printf("Supported levels: INFO, WARNING, ERROR\n");
    printf("Enter 'quit' to finish input and analyze logs.\n");
    
    while (1) {
        printf("> ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
        
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        
        if (strcmp(input_buffer, "quit") == 0) break;
        if (strlen(input_buffer) == 0) continue;
        
        int parsed = sscanf(input_buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
            continue;
        }
        
        if (strcmp(level, "INFO") != 0 && strcmp(level, "WARNING") != 0 && strcmp(level, "ERROR") != 0) {
            printf("Invalid level. Use INFO, WARNING, or ERROR.\n");
            continue;
        }
        
        if (!add_log_entry(&heap, timestamp, level, message)) {
            printf("Failed to add log entry (storage full or invalid data).\n");
            break;
        }
    }
    
    analyze_logs(&heap);
    free_log_heap(&heap);
    
    return 0;
}