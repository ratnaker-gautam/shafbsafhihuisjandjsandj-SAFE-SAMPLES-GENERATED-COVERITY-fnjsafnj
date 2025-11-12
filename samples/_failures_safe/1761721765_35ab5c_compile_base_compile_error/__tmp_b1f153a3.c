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
    size_t count;
    size_t capacity;
} LogBuffer;

void init_log_buffer(LogBuffer* buffer, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 10;
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (buffer->entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    buffer->count = 0;
    buffer->capacity = initial_capacity;
}

void add_log_entry(LogBuffer* buffer, const char* timestamp, const char* level, const char* message) {
    if (buffer->count >= buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        LogEntry* new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
        if (new_entries == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            return;
        }
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    if (buffer->count < buffer->capacity) {
        LogEntry* entry = &buffer->entries[buffer->count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        buffer->count++;
    }
}

void free_log_buffer(LogBuffer* buffer) {
    free(buffer->entries);
    buffer->entries = NULL;
    buffer->count = 0;
    buffer->capacity = 0;
}

void analyze_logs(LogBuffer* buffer) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < buffer->count; i++) {
        const LogEntry* entry = &buffer->entries[i];
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", buffer->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        int printed = 0;
        for (size_t i = buffer->count; i > 0 && printed < 3; i--) {
            const LogEntry* entry = &buffer->entries[i-1];
            if (strcmp(entry->level, "ERROR") == 0) {
                printf("- %s: %s\n", entry->timestamp, entry->message);
                printed++;
            }
        }
    }
}

int main() {
    LogBuffer heap_buffer;
    LogEntry stack_entries[50];
    size_t stack_count = 0;
    
    init_log_buffer(&heap_buffer, 20);
    
    printf("Enter log entries (format: timestamp level message). Type 'done' to finish.\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "done", 4) == 0) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        char timestamp[32] = "";
        char level[16] = "";
        char message[256] = "";
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed < 3) {
            printf("Invalid format. Use: timestamp level message\n");
            continue;
        }
        
        if (stack_count < sizeof(stack_entries) / sizeof(stack_entries[0])) {
            LogEntry* entry = &stack_entries[stack_count];
            strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
            entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
            strncpy(entry->level, level, sizeof(entry->level) - 1);
            entry->level[sizeof(entry->level) - 1