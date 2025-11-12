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
    
    char level_buf[16];
    char timestamp_buf[32];
    char message_buf[256];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                       timestamp_buf, level_buf, message_buf);
    
    if (parsed != 3) return 0;
    
    if (strlen(timestamp_buf) >= sizeof(entry->timestamp) ||
        strlen(level_buf) >= sizeof(entry->level) ||
        strlen(message_buf) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp_buf);
    strcpy(entry->level, level_buf);
    strcpy(entry->message, message_buf);
    entry->count = 1;
    
    return 1;
}

void analyze_logs(LogBuffer* buffer) {
    if (!buffer || buffer->count == 0) return;
    
    uint32_t info_count = 0;
    uint32_t warn_count = 0;
    uint32_t error_count = 0;
    char most_common_level[16] = "";
    uint32_t max_level_count = 0;
    uint32_t level_counts[3] = {0};
    const char* levels[] = {"INFO", "WARN", "ERROR"};
    
    for (size_t i = 0; i < buffer->count; i++) {
        if (strcmp(buffer->entries[i].level, "INFO") == 0) {
            info_count += buffer->entries[i].count;
            level_counts[0] += buffer->entries[i].count;
        } else if (strcmp(buffer->entries[i].level, "WARN") == 0) {
            warn_count += buffer->entries[i].count;
            level_counts[1] += buffer->entries[i].count;
        } else if (strcmp(buffer->entries[i].level, "ERROR") == 0) {
            error_count += buffer->entries[i].count;
            level_counts[2] += buffer->entries[i].count;
        }
    }
    
    for (int i = 0; i < 3; i++) {
        if (level_counts[i] > max_level_count) {
            max_level_count = level_counts[i];
            strcpy(most_common_level, levels[i]);
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", buffer->count);
    printf("INFO count: %u\n", info_count);
    printf("WARN count: %u\n", warn_count);
    printf("ERROR count: %u\n", error_count);
    printf("Most common level: %s\n", most_common_level);
}

int main(void) {
    LogBuffer stack_buffer;
    stack_buffer.count = 0;
    
    LogBuffer* heap_buffer = malloc(sizeof(LogBuffer));
    if (!heap_buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    heap_buffer->count = 0;
    
    char line[MAX_LINE_LENGTH];
    size_t line_count = 0;
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        if (line_count > 100) {
            fprintf(stderr, "Too many input lines\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
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
        } else {
            fprintf(stderr, "Failed to parse line: %s\n", line);
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        free(heap_buffer);
        return 1;
    }
    
    printf("\nStack Buffer Analysis:\n");
    analyze_logs(&stack_buffer);
    
    printf("\nHeap Buffer Analysis:\n");
    analyze_logs(heap_buffer);
    
    free(heap_buffer);
    return 0;
}