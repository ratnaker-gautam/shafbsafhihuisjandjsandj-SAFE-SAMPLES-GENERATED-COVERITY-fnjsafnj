//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->count = 1;
    
    return 1;
}

void count_log_levels(LogBuffer* buffer, uint32_t* counts) {
    if (!buffer || !counts) return;
    
    for (size_t i = 0; i < 4; i++) counts[i] = 0;
    
    for (size_t i = 0; i < buffer->count; i++) {
        const char* level = buffer->entries[i].level;
        if (strcmp(level, "INFO") == 0) counts[0]++;
        else if (strcmp(level, "WARN") == 0) counts[1]++;
        else if (strcmp(level, "ERROR") == 0) counts[2]++;
        else counts[3]++;
    }
}

int main() {
    LogBuffer stack_buffer;
    stack_buffer.count = 0;
    
    LogBuffer* heap_buffer = malloc(sizeof(LogBuffer));
    if (!heap_buffer) {
        printf("Memory allocation failed\n");
        return 1;
    }
    heap_buffer->count = 0;
    
    char line[MAX_LINE_LENGTH];
    size_t line_count = 0;
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0) break;
        
        line_count++;
        if (line_count > 100) {
            printf("Maximum input lines exceeded\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (stack_buffer.count < MAX_ENTRIES) {
                stack_buffer.entries[stack_buffer.count] = entry;
                stack_buffer.count++;
            }
            
            if (heap_buffer->count < MAX_ENTRIES) {
                heap_buffer->entries[heap_buffer->count] = entry;
                heap_buffer->count++;
            }
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total log entries: %zu\n", stack_buffer.count);
    
    uint32_t stack_counts[4];
    count_log_levels(&stack_buffer, stack_counts);
    
    printf("INFO: %u\n", stack_counts[0]);
    printf("WARN: %u\n", stack_counts[1]);
    printf("ERROR: %u\n", stack_counts[2]);
    printf("OTHER: %u\n", stack_counts[3]);
    
    uint32_t heap_counts[4];
    count_log_levels(heap_buffer, heap_counts);
    
    printf("Heap buffer verification - INFO: %u\n", heap_counts[0]);
    
    free(heap_buffer);
    
    return 0;
}