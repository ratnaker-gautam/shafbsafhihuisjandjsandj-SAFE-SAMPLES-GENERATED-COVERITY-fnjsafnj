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
    int severity;
} LogEntry;

typedef struct {
    LogEntry *entries;
    int count;
    int capacity;
} LogBuffer;

int parse_severity(const char *level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_timestamp(const char *timestamp) {
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

int add_log_entry(LogBuffer *buffer, const char *line) {
    if (buffer == NULL || line == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    if (!validate_timestamp(timestamp)) return 0;
    
    int severity = parse_severity(level);
    if (severity == -1) return 0;
    
    if (buffer->count >= buffer->capacity) {
        int new_capacity = buffer->capacity * 2;
        if (new_capacity <= 0) new_capacity = 16;
        LogEntry *new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
        if (new_entries == NULL) return 0;
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    LogEntry *entry = &buffer->entries[buffer->count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = severity;
    buffer->count++;
    
    return 1;
}

void print_statistics(const LogBuffer *buffer) {
    if (buffer == NULL || buffer->count == 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    int severity_count[5] = {0};
    for (int i = 0; i < buffer->count; i++) {
        if (buffer->entries[i].severity >= 0 && buffer->entries[i].severity < 5) {
            severity_count[buffer->entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("CRITICAL: %d\n", severity_count[4]);
}

void cleanup_buffer(LogBuffer *buffer) {
    if (buffer != NULL) {
        free(buffer->entries);
        buffer->entries = NULL;
        buffer->count = 0;
        buffer->capacity = 0;
    }
}

int main(void) {
    LogBuffer buffer_stack;
    LogBuffer *buffer = &buffer_stack;
    buffer->entries = NULL;
    buffer->count = 0;
    buffer->capacity = 0;
    
    char input_line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (fgets(input_line, sizeof(input_line), stdin) != NULL) {
        if (line_count >= MAX_ENTRIES) {
            printf("Maximum number of entries reached.\n");
            break;
        }
        
        size_t len = strlen(input_line