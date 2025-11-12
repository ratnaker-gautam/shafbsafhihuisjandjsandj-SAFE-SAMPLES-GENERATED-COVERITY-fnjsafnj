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
} LogEntry;

typedef struct {
    LogEntry* entries;
    int count;
    int capacity;
} LogBuffer;

int parse_timestamp(const char* ts_str, struct tm* tm) {
    return sscanf(ts_str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 ||
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

LogBuffer* create_log_buffer(int initial_capacity) {
    if (initial_capacity <= 0 || initial_capacity > 10000) {
        return NULL;
    }
    
    LogBuffer* buffer = malloc(sizeof(LogBuffer));
    if (!buffer) return NULL;
    
    buffer->entries = malloc(sizeof(LogEntry) * initial_capacity);
    if (!buffer->entries) {
        free(buffer);
        return NULL;
    }
    
    buffer->count = 0;
    buffer->capacity = initial_capacity;
    return buffer;
}

void destroy_log_buffer(LogBuffer* buffer) {
    if (buffer) {
        free(buffer->entries);
        free(buffer);
    }
}

int add_log_entry(LogBuffer* buffer, const char* timestamp, const char* level, const char* message) {
    if (!buffer || !timestamp || !level || !message) return 0;
    
    if (buffer->count >= buffer->capacity) {
        int new_capacity = buffer->capacity * 2;
        if (new_capacity > 10000) new_capacity = 10000;
        
        LogEntry* new_entries = realloc(buffer->entries, sizeof(LogEntry) * new_capacity);
        if (!new_entries) return 0;
        
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    LogEntry* entry = &buffer->entries[buffer->count];
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    buffer->count++;
    return 1;
}

void analyze_logs(LogBuffer* buffer) {
    if (!buffer || buffer->count == 0) {
        printf("No logs to analyze.\n");
        return;
    }
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        const char* level = buffer->entries[i].level;
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
}

int main() {
    LogBuffer* buffer = create_log_buffer(10);
    if (!buffer) {
        printf("Failed to create log buffer.\n");
        return 1;
    }
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Enter 'quit' to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "quit", 4) == 0) break;
        
        char timestamp[32], level[16], message[256];
        struct tm tm = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE\n");
            continue;
        }
        
        if (!parse_timestamp(timestamp, &tm)) {
            printf("Invalid timestamp format.\n");
            continue;
        }
        
        if (!is_valid_level(level)) {
            printf("Invalid log level. Use: INFO, WARN, ERROR, DEBUG\n");
            continue;