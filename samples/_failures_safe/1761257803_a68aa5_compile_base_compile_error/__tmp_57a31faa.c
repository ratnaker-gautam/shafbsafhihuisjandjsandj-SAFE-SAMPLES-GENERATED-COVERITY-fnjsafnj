//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int init_log_buffer(LogBuffer* buffer, size_t initial_capacity) {
    if (!buffer || initial_capacity == 0) return 0;
    
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!buffer->entries) return 0;
    
    buffer->count = 0;
    buffer->capacity = initial_capacity;
    return 1;
}

int add_to_log_buffer(LogBuffer* buffer, const LogEntry* entry) {
    if (!buffer || !entry) return 0;
    
    if (buffer->count >= buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        
        LogEntry* new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
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

void analyze_log_levels(const LogBuffer* buffer) {
    if (!buffer) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < buffer->count; i++) {
        const char* level = buffer->entries[i].level;
        
        if (strcmp(level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Level Analysis:\n");
    printf("  INFO: %d entries\n", info_count);
    printf("  WARNING: %d entries\n", warning_count);
    printf("  ERROR: %d entries\n", error_count);
}

void cleanup_log_buffer(LogBuffer* buffer) {
    if (buffer && buffer->entries) {
        free(buffer->entries);
        buffer->entries = NULL;
        buffer->count = 0;
        buffer->capacity = 0;
    }
}

int main() {
    LogBuffer buffer;
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    if (!init_log_buffer(&buffer, 10)) {
        fprintf(stderr, "Failed to initialize log buffer\n");
        return 1;
    }
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO Application started\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        
        if (line_count > MAX_ENTRIES) {
            printf("Maximum number of entries reached.\n");
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (!add_to_log_buffer(&buffer, &entry)) {
                fprintf(stderr, "Failed to add log entry to buffer\n");
                break;
            }
        } else {
            fprintf(stderr, "Failed to parse line: %s\n", line);
        }
    }
    
    if (buffer.count > 0) {
        printf("\nAnalysis Results:\n");
        printf("Total entries processed: %zu\n", buffer.count);
        analyze_log_levels(&buffer);
        
        printf("\nRecent entries:\n");
        size_t display_count = (buffer.count > 5) ? 5 : buffer.count;
        for (size_t i = buffer.count - display_count