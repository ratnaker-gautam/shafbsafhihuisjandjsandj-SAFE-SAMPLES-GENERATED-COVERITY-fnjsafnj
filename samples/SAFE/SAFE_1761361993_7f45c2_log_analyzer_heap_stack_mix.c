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
    LogEntry entries[MAX_ENTRIES];
    int count;
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

void analyze_logs(LogBuffer* buffer) {
    if (!buffer || buffer->count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        if (strcmp(buffer->entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(buffer->entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(buffer->entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
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
    int line_count = 0;
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0) {
            break;
        }
        
        line_count++;
        if (line_count > MAX_ENTRIES * 2) {
            fprintf(stderr, "Too many input lines\n");
            free(heap_buffer);
            return 1;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (line_count % 2 == 0) {
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
        }
    }
    
    printf("\nStack Buffer Analysis:\n");
    analyze_logs(&stack_buffer);
    
    printf("\nHeap Buffer Analysis:\n");
    analyze_logs(heap_buffer);
    
    printf("\nCombined Analysis:\n");
    LogEntry combined[MAX_ENTRIES * 2];
    int combined_count = 0;
    
    for (int i = 0; i < stack_buffer.count && combined_count < MAX_ENTRIES * 2; i++) {
        combined[combined_count++] = stack_buffer.entries[i];
    }
    
    for (int i = 0; i < heap_buffer->count && combined_count < MAX_ENTRIES * 2; i++) {
        combined[combined_count++] = heap_buffer->entries[i];
    }
    
    LogBuffer final_buffer;
    final_buffer.count = combined_count;
    for (int i = 0; i < combined_count; i++) {
        final_buffer.entries[i] = combined[i];
    }
    
    analyze_logs(&final_buffer);
    
    free(heap_buffer);
    return 0;
}