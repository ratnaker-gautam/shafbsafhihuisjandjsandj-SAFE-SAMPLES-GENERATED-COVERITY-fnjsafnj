//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    size_t count;
} LogBuffer;

int parse_log_line(const char* line, LogEntry* entry) {
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

void analyze_logs(LogBuffer* buffer) {
    if (!buffer || buffer->count == 0) return;
    
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
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", buffer->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        for (size_t i = 0; i < buffer->count; i++) {
            if (strcmp(buffer->entries[i].level, "ERROR") == 0) {
                printf("%s: %s\n", buffer->entries[i].timestamp, buffer->entries[i].message);
            }
        }
    }
}

int main() {
    LogBuffer stack_buffer;
    stack_buffer.count = 0;
    
    LogBuffer* heap_buffer = malloc(sizeof(LogBuffer));
    if (!heap_buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    heap_buffer->count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    size_t total_entries = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (total_entries < MAX_ENTRIES) {
                if (total_entries % 2 == 0) {
                    if (stack_buffer.count < MAX_ENTRIES) {
                        stack_buffer.entries[stack_buffer.count] = entry;
                        stack_buffer.count++;
                    }
                } else {
                    if (heap_buffer->count < MAX_ENTRIES) {
                        heap_buffer->entries[heap_buffer->count] = entry;
                        heap_buffer->count++;
                    }
                }
                total_entries++;
            } else {
                printf("Maximum log entries reached\n");
                break;
            }
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    printf("\nAnalyzing stack-stored logs (%zu entries):\n", stack_buffer.count);
    analyze_logs(&stack_buffer);
    
    printf("\nAnalyzing heap-stored logs (%zu entries):\n", heap_buffer->count);
    analyze_logs(heap_buffer);
    
    free(heap_buffer);
    return 0;
}