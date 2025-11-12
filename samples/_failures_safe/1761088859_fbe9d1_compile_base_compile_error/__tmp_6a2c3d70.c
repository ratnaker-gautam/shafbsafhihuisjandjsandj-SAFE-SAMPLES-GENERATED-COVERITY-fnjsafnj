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

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char level[16];
    char timestamp[32];
    char message[256];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char *level) {
    const char *valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

void init_log_buffer(LogBuffer *buffer, int initial_capacity) {
    if (!buffer || initial_capacity <= 0) return;
    
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!buffer->entries) return;
    
    buffer->count = 0;
    buffer->capacity = initial_capacity;
}

void add_to_buffer(LogBuffer *buffer, const LogEntry *entry) {
    if (!buffer || !entry || buffer->count >= buffer->capacity) return;
    
    if (buffer->count < buffer->capacity) {
        buffer->entries[buffer->count] = *entry;
        buffer->count++;
    }
}

void analyze_logs(LogBuffer *buffer) {
    if (!buffer || buffer->count == 0) return;
    
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        const char *level = buffer->entries[i].level;
        
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARNING") == 0) warning_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (error_count > 0) {
        printf("\nError entries:\n");
        for (int i = 0; i < buffer->count; i++) {
            if (strcmp(buffer->entries[i].level, "ERROR") == 0) {
                printf("%s: %s\n", buffer->entries[i].timestamp, buffer->entries[i].message);
            }
        }
    }
}

void cleanup_buffer(LogBuffer *buffer) {
    if (buffer && buffer->entries) {
        free(buffer->entries);
        buffer->entries = NULL;
        buffer->count = 0;
        buffer->capacity = 0;
    }
}

int main() {
    LogBuffer buffer;
    init_log_buffer(&buffer, 100);
    
    if (!buffer.entries) {
        fprintf(stderr, "Failed to initialize log buffer\n");
        return 1;
    }
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR, DEBUG\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                if (buffer.count < buffer.capacity) {
                    add_to_buffer(&buffer, &entry);
                    entry_count++;
                } else {
                    printf("Buffer full, cannot add more entries\n");
                    break;
                }
            } else {
                printf("Invalid log level: %s\n", entry.level);
            }
        } else {
            printf("Invalid log format: %s\n", line);
        }
        
        if (entry_count >= MAX_ENTRIES) {
            printf("Maximum entry count reached\n");
            break;
        }
    }
    
    if (buffer.count > 0) {
        analyze_logs(&buffer);
    } else {
        printf("No valid log entries to analyze\n");
    }