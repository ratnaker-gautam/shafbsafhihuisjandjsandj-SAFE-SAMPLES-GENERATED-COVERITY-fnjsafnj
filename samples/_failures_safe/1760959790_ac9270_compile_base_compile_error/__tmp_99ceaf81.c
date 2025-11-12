//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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
    LogEntry* entries;
    size_t count;
    size_t capacity;
} LogBuffer;

int parse_timestamp(const char* str, struct tm* tm) {
    return sscanf(str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 ||
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

int parse_log_line(const char* line, LogEntry* entry) {
    char level[16];
    char timestamp[32];
    char message[256];
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    struct tm tm = {0};
    if (!parse_timestamp(timestamp, &tm)) {
        return 0;
    }
    
    if (!is_valid_level(level)) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void init_log_buffer(LogBuffer* buffer, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_ENTRIES) {
        initial_capacity = 10;
    }
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (buffer->entries) {
        buffer->count = 0;
        buffer->capacity = initial_capacity;
    } else {
        buffer->count = 0;
        buffer->capacity = 0;
    }
}

void free_log_buffer(LogBuffer* buffer) {
    if (buffer->entries) {
        free(buffer->entries);
        buffer->entries = NULL;
    }
    buffer->count = 0;
    buffer->capacity = 0;
}

int add_to_log_buffer(LogBuffer* buffer, const LogEntry* entry) {
    if (!buffer->entries || buffer->count >= MAX_ENTRIES) {
        return 0;
    }
    
    if (buffer->count >= buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity > MAX_ENTRIES) {
            new_capacity = MAX_ENTRIES;
        }
        LogEntry* new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) {
            return 0;
        }
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    buffer->entries[buffer->count] = *entry;
    buffer->count++;
    return 1;
}

void analyze_logs(const LogBuffer* buffer) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (size_t i = 0; i < buffer->count; i++) {
        const LogEntry* entry = &buffer->entries[i];
        if (strcmp(entry->level, "INFO") == 0) info_count++;
        else if (strcmp(entry->level, "WARN") == 0) warn_count++;
        else if (strcmp(entry->level, "ERROR") == 0) error_count++;
        else if (strcmp(entry->level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", buffer->count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
}

int main(void) {
    LogBuffer buffer;
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    init_log_buffer(&buffer, 10);
    if (!buffer.entries) {
        fprintf(stderr, "Failed to initialize log buffer\n");
        return 1;
    }
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: INFO, WARN, ERROR, DEBUG\n");
    printf("Enter empty line to finish\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        if (line_count > 100