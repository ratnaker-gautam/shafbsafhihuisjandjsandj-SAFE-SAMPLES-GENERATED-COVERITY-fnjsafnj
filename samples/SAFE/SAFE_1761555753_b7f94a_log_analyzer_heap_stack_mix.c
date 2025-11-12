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

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_log_line(const char* line) {
    if (line == NULL) return 0;
    size_t len = strlen(line);
    if (len == 0 || len >= MAX_LINE_LENGTH) return 0;
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (!validate_log_line(line) || entry == NULL) return 0;
    
    char timestamp[32], level[16], message[256];
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    
    if (parsed != 3) return 0;
    
    int severity = parse_severity(level);
    if (severity == -1) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = severity;
    
    return 1;
}

void analyze_logs(LogBuffer* buffer) {
    if (buffer == NULL || buffer->count == 0) return;
    
    int severity_count[5] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        if (buffer->entries[i].severity >= 0 && buffer->entries[i].severity < 5) {
            severity_count[buffer->entries[i].severity]++;
            total_severity += buffer->entries[i].severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("CRITICAL: %d\n", severity_count[4]);
    
    if (buffer->count > 0) {
        double avg_severity = (double)total_severity / buffer->count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

int main(void) {
    LogBuffer* buffer = malloc(sizeof(LogBuffer));
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    buffer->count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    while (buffer->count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (!validate_log_line(line)) {
            fprintf(stderr, "Invalid log line format\n");
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (buffer->count < MAX_ENTRIES) {
                buffer->entries[buffer->count] = entry;
                buffer->count++;
            } else {
                fprintf(stderr, "Log buffer full\n");
                break;
            }
        } else {
            fprintf(stderr, "Failed to parse log line: %s\n", line);
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