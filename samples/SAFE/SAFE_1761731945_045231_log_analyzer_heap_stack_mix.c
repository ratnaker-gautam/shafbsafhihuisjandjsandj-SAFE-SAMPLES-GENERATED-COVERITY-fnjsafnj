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
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
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
        int printed = 0;
        for (int i = buffer->count - 1; i >= 0 && printed < 3; i--) {
            if (strcmp(buffer->entries[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", buffer->entries[i].timestamp, buffer->entries[i].message);
                printed++;
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
    
    char sample_logs[][MAX_LINE_LENGTH] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:35:23 WARNING High memory usage detected",
        "2024-01-15T10:40:15 ERROR Database connection failed",
        "2024-01-15T10:42:10 INFO Backup completed",
        "2024-01-15T10:45:30 ERROR File system full",
        "2024-01-15T10:50:00 INFO User login successful",
        "2024-01-15T10:55:45 WARNING Slow response time",
        "2024-01-15T11:00:00 INFO Scheduled task completed"
    };
    
    int num_samples = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < num_samples && buffer->count < MAX_ENTRIES; i++) {
        LogEntry entry;
        if (parse_log_line(sample_logs[i], &entry)) {
            buffer->entries[buffer->count] = entry;
            buffer->count++;
        }
    }
    
    analyze_logs(buffer);
    
    free(buffer);
    return 0;
}