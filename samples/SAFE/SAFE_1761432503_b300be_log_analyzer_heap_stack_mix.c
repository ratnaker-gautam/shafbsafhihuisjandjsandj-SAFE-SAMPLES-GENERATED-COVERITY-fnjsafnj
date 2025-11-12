//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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
    int severity;
} LogEntry;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    int count;
} LogBuffer;

LogBuffer* create_log_buffer() {
    LogBuffer* buffer = malloc(sizeof(LogBuffer));
    if (buffer == NULL) {
        return NULL;
    }
    buffer->count = 0;
    return buffer;
}

void destroy_log_buffer(LogBuffer* buffer) {
    free(buffer);
}

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int add_log_entry(LogBuffer* buffer, const char* timestamp, const char* level, const char* message) {
    if (buffer == NULL || timestamp == NULL || level == NULL || message == NULL) {
        return 0;
    }
    if (buffer->count >= MAX_ENTRIES) {
        return 0;
    }
    
    LogEntry* entry = &buffer->entries[buffer->count];
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->severity = parse_log_level(level);
    
    buffer->count++;
    return 1;
}

void analyze_logs(LogBuffer* buffer) {
    if (buffer == NULL || buffer->count == 0) {
        printf("No logs to analyze\n");
        return;
    }
    
    int severity_count[6] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        LogEntry* entry = &buffer->entries[i];
        if (entry->severity >= 1 && entry->severity <= 5) {
            severity_count[entry->severity]++;
            total_severity += entry->severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("DEBUG: %d\n", severity_count[1]);
    printf("INFO: %d\n", severity_count[2]);
    printf("WARNING: %d\n", severity_count[3]);
    printf("ERROR: %d\n", severity_count[4]);
    printf("CRITICAL: %d\n", severity_count[5]);
    
    if (buffer->count > 0) {
        double avg_severity = (double)total_severity / buffer->count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

int main() {
    LogBuffer* buffer = create_log_buffer();
    if (buffer == NULL) {
        printf("Failed to create log buffer\n");
        return 1;
    }
    
    printf("Enter log entries (format: timestamp level message)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Type 'done' to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "done", 4) == 0) {
            break;
        }
        
        char timestamp[32];
        char level[16];
        char message[256];
        
        int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (result == 3) {
            if (add_log_entry(buffer, timestamp, level, message)) {
                printf("Added: %s %s %s\n", timestamp, level, message);
            } else {
                printf("Failed to add log entry\n");
            }
        } else {
            printf("Invalid format. Use: timestamp level message\n");
        }
        
        if (buffer->count >= MAX_ENTRIES) {
            printf("Maximum entries reached\n");
            break;
        }
    }
    
    analyze_logs(buffer);
    destroy_log_buffer(buffer);
    
    return 0;
}