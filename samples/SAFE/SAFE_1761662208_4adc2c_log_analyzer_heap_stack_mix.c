//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
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
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
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
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        for (int i = 0; i < buffer->count && i < 5; i++) {
            if (strcmp(buffer->entries[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", buffer->entries[i].timestamp, buffer->entries[i].message);
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
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LEN];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (entry_count < MAX_ENTRIES) {
                if (entry_count % 2 == 0) {
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
                entry_count++;
            }
        } else {
            fprintf(stderr, "Invalid log format: %s\n", line);
        }
    }
    
    printf("\nStack buffer analysis:\n");
    analyze_logs(&stack_buffer);
    
    printf("\nHeap buffer analysis:\n");
    analyze_logs(heap_buffer);
    
    printf("\nCombined analysis:\n");
    LogBuffer combined;
    combined.count = 0;
    
    for (int i = 0; i < stack_buffer.count && combined.count < MAX_ENTRIES; i++) {
        combined.entries[combined.count] = stack_buffer.entries[i];
        combined.count++;
    }
    
    for (int i = 0; i < heap_buffer->count && combined.count < MAX_ENTRIES; i++) {
        combined.entries[combined.count] = heap_buffer->entries[i];
        combined.count++;
    }
    
    analyze_logs(&combined);
    
    free(heap_buffer);
    return 0;
}