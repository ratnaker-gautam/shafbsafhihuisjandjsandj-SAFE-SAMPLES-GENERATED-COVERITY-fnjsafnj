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
} LogBuffer;

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

LogBuffer* create_log_buffer(size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_ENTRIES) return NULL;
    
    LogBuffer *buffer = malloc(sizeof(LogBuffer));
    if (!buffer) return NULL;
    
    buffer->entries = malloc(sizeof(LogEntry) * initial_capacity);
    if (!buffer->entries) {
        free(buffer);
        return NULL;
    }
    
    buffer->count = 0;
    buffer->capacity = initial_capacity;
    return buffer;
}

int add_log_entry(LogBuffer *buffer, const LogEntry *entry) {
    if (!buffer || !entry || buffer->count >= MAX_ENTRIES) return 0;
    
    if (buffer->count >= buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        
        LogEntry *new_entries = realloc(buffer->entries, sizeof(LogEntry) * new_capacity);
        if (!new_entries) return 0;
        
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    if (buffer->count < buffer->capacity) {
        buffer->entries[buffer->count] = *entry;
        buffer->count++;
        return 1;
    }
    
    return 0;
}

void analyze_logs(const LogBuffer *buffer) {
    if (!buffer || buffer->count == 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < buffer->count; i++) {
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
    printf("Total entries: %zu\n", buffer->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
}

void free_log_buffer(LogBuffer *buffer) {
    if (buffer) {
        free(buffer->entries);
        free(buffer);
    }
}

int main() {
    LogBuffer *buffer = create_log_buffer(10);
    if (!buffer) {
        fprintf(stderr, "Failed to create log buffer\n");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (!add_log_entry(buffer, &entry)) {
                fprintf(stderr, "Failed to add log entry\n");
                break;
            }
        } else {
            fprintf(stderr, "Failed to parse log line: %s\n", line);
        }
        
        if (buffer->count >= MAX_ENTRIES) {
            printf("Maximum log entries reached\n");
            break;
        }
    }
    
    if (buffer->count > 0) {
        analyze_logs(buffer);
    } else {
        printf("No valid log entries to analyze\n");
    }
    
    free_log_buffer(buffer);
    return 0;
}