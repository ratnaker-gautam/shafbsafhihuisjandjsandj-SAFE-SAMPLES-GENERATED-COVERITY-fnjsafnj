//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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
    LogEntry* entries;
    int count;
    int capacity;
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
    if (!line || strlen(line) < 10) return 0;
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char level_buf[16];
    char message_buf[256];
    char timestamp_buf[32];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp_buf, level_buf, message_buf);
    if (parsed != 3) return 0;
    
    int severity = parse_severity(level_buf);
    if (severity == -1) return 0;
    
    strncpy(entry->timestamp, timestamp_buf, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = severity;
    
    return 1;
}

int init_log_buffer(LogBuffer* buffer, int initial_capacity) {
    if (!buffer || initial_capacity <= 0 || initial_capacity > MAX_ENTRIES) return 0;
    
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!buffer->entries) return 0;
    
    buffer->count = 0;
    buffer->capacity = initial_capacity;
    return 1;
}

int add_to_buffer(LogBuffer* buffer, const LogEntry* entry) {
    if (!buffer || !entry || buffer->count >= MAX_ENTRIES) return 0;
    
    if (buffer->count >= buffer->capacity) {
        int new_capacity = buffer->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        
        LogEntry* new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    buffer->entries[buffer->count] = *entry;
    buffer->count++;
    return 1;
}

void analyze_logs(LogBuffer* buffer) {
    if (!buffer || buffer->count == 0) return;
    
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

void cleanup_buffer(LogBuffer* buffer) {
    if (buffer && buffer->entries) {
        free(buffer->entries);
        buffer->entries = NULL;
        buffer->count = 0;
        buffer->capacity = 0;
    }
}

int main() {
    LogBuffer buffer;
    LogEntry stack_entry;
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    if (!init_log_buffer(&buffer, 10)) {
        fprintf(stderr, "Failed to initialize log buffer\n");
        return 1;
    }
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-