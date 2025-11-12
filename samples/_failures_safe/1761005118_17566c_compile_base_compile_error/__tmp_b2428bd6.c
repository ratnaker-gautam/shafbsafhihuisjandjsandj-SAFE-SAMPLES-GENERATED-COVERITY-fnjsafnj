//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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
    int severity;
} LogEntry;

typedef struct {
    LogEntry* entries;
    size_t count;
    size_t capacity;
} LogBuffer;

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int is_valid_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    if (!timestamp || !is_valid_timestamp(timestamp)) return 0;
    
    char* level = strtok(NULL, " ");
    if (!level) return 0;
    
    int severity = parse_severity(level);
    if (severity == -1) return 0;
    
    char* message = strtok(NULL, "");
    if (!message) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = severity;
    
    return 1;
}

int init_log_buffer(LogBuffer* buffer, size_t initial_capacity) {
    if (!buffer || initial_capacity == 0) return 0;
    
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!buffer->entries) return 0;
    
    buffer->count = 0;
    buffer->capacity = initial_capacity;
    return 1;
}

int add_to_buffer(LogBuffer* buffer, const LogEntry* entry) {
    if (!buffer || !entry) return 0;
    
    if (buffer->count >= buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity <= buffer->capacity) return 0;
        
        LogEntry* new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    buffer->entries[buffer->count] = *entry;
    buffer->count++;
    return 1;
}

void analyze_logs(const LogBuffer* buffer) {
    if (!buffer || buffer->count == 0) {
        printf("No logs to analyze.\n");
        return;
    }
    
    int severity_count[5] = {0};
    char latest_timestamp[32] = "";
    
    for (size_t i = 0; i < buffer->count; i++) {
        const LogEntry* entry = &buffer->entries[i];
        if (entry->severity >= 0 && entry->severity < 5) {
            severity_count[entry->severity]++;
        }
        if (strcmp(entry->timestamp, latest_timestamp) > 0) {
            strncpy(latest_timestamp, entry->timestamp, sizeof(latest_timestamp) - 1);
            latest_timestamp[sizeof(latest_timestamp) - 1] = '\0';
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", buffer->count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("CRITICAL: %d\n", severity_count[4]);
    printf("Latest timestamp: %s\n", latest_timestamp);