//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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
} LogBuffer;

void init_log_buffer(LogBuffer *buffer, int initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = 10;
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!buffer->entries) exit(EXIT_FAILURE);
    buffer->count = 0;
    buffer->capacity = initial_capacity;
}

void add_log_entry(LogBuffer *buffer, const char *timestamp, const char *level, const char *message) {
    if (buffer->count >= buffer->capacity) {
        int new_capacity = buffer->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        LogEntry *new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) exit(EXIT_FAILURE);
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    if (buffer->count < buffer->capacity) {
        LogEntry *entry = &buffer->entries[buffer->count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        buffer->count++;
    }
}

void analyze_logs(LogBuffer *buffer) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        const LogEntry *entry = &buffer->entries[i];
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
}

void free_log_buffer(LogBuffer *buffer) {
    free(buffer->entries);
    buffer->entries = NULL;
    buffer->count = 0;
    buffer->capacity = 0;
}

int main() {
    LogBuffer heap_buffer;
    LogEntry stack_entries[50];
    int stack_count = 0;
    
    init_log_buffer(&heap_buffer, 20);
    
    printf("Enter log entries (format: timestamp level message)\n");
    printf("Type 'done' to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "done", 4) == 0) break;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed >= 2) {
            if (stack_count < 50) {
                LogEntry *entry = &stack_entries[stack_count];
                strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
                entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
                strncpy(entry->level, level, sizeof(entry->level) - 1);
                entry->level[sizeof(entry->level) - 1] = '\0';
                if (parsed == 3) {
                    strncpy(entry->message, message, sizeof(entry->message) - 1);
                    entry->message[sizeof(entry->message) - 1] = '\0';
                }
                stack_count++;
            }
            
            add_log_entry(&heap_buffer, timestamp, level, message);
        }
    }
    
    printf("\nStack buffer analysis (%d entries):\n", stack_count);
    for (int i = 0; i < stack_count; i++) {
        printf("%s %s %s\n", stack_entries[i].timestamp, stack_entries[i].level, stack_entries[i].message);
    }
    
    printf("\nHeap buffer analysis:\n");
    analyze_logs(&heap_buffer);
    
    free_log_buffer(&heap_buffer);
    return 0;
}