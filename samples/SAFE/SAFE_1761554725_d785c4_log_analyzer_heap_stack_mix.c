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
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
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
    LogBuffer* buffer = malloc(sizeof(LogBuffer));
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    buffer->count = 0;
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        if (line_count > 1000) {
            printf("Maximum input lines reached\n");
            break;
        }
        
        if (strlen(line) > 0 && line[strlen(line)-1] == '\n') {
            line[strlen(line)-1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (buffer->count >= MAX_ENTRIES) {
            printf("Maximum log entries reached\n");
            break;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            buffer->entries[buffer->count] = entry;
            buffer->count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (buffer->count > 0) {
        analyze_logs(buffer);
    } else {
        printf("No valid log entries to analyze\n");
    }
    
    free(buffer);
    return 0;
}