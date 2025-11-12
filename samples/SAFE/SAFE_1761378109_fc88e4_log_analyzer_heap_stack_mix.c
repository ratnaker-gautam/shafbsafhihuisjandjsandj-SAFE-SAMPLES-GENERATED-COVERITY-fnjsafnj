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
    uint32_t count;
} LogEntry;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    size_t count;
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
    entry->count = 1;
    
    return 1;
}

void analyze_logs(LogBuffer* buffer) {
    if (!buffer || buffer->count == 0) return;
    
    uint32_t error_count = 0;
    uint32_t warning_count = 0;
    uint32_t info_count = 0;
    
    for (size_t i = 0; i < buffer->count; i++) {
        if (strcmp(buffer->entries[i].level, "ERROR") == 0) {
            error_count += buffer->entries[i].count;
        } else if (strcmp(buffer->entries[i].level, "WARNING") == 0) {
            warning_count += buffer->entries[i].count;
        } else if (strcmp(buffer->entries[i].level, "INFO") == 0) {
            info_count += buffer->entries[i].count;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %u\n", error_count);
    printf("WARNING entries: %u\n", warning_count);
    printf("INFO entries: %u\n", info_count);
    printf("Total unique entries: %zu\n", buffer->count);
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
    
    char line[MAX_LINE_LENGTH];
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            int found = 0;
            for (size_t i = 0; i < stack_buffer.count; i++) {
                if (strcmp(stack_buffer.entries[i].timestamp, entry.timestamp) == 0 &&
                    strcmp(stack_buffer.entries[i].level, entry.level) == 0 &&
                    strcmp(stack_buffer.entries[i].message, entry.message) == 0) {
                    stack_buffer.entries[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found && stack_buffer.count < MAX_ENTRIES) {
                stack_buffer.entries[stack_buffer.count] = entry;
                stack_buffer.count++;
            }
            
            if (heap_buffer->count < MAX_ENTRIES) {
                heap_buffer->entries[heap_buffer->count] = entry;
                heap_buffer->count++;
            }
        } else {
            fprintf(stderr, "Invalid log format: %s\n", line);
        }
    }
    
    printf("\nStack Buffer Analysis:\n");
    analyze_logs(&stack_buffer);
    
    printf("\nHeap Buffer Analysis:\n");
    analyze_logs(heap_buffer);
    
    free(heap_buffer);
    return 0;
}