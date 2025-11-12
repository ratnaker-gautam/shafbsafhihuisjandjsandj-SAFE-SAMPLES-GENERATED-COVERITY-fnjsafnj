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
} LogEntry;

typedef struct {
    LogEntry* entries;
    int count;
    int capacity;
} LogBuffer;

int parse_timestamp(const char* str, struct tm* tm) {
    return sscanf(str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int validate_log_level(const char* level) {
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 ||
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

LogBuffer* create_log_buffer(int initial_capacity) {
    if (initial_capacity <= 0 || initial_capacity > MAX_ENTRIES) {
        return NULL;
    }
    
    LogBuffer* buffer = malloc(sizeof(LogBuffer));
    if (!buffer) return NULL;
    
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
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
    if (buffer->count >= buffer->capacity) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) {
        return 0;
    }
    
    struct tm tm;
    if (!parse_timestamp(timestamp, &tm)) return 0;
    if (!validate_log_level(level)) return 0;
    
    LogEntry* entry = &buffer->entries[buffer->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    buffer->count++;
    return 1;
}

void analyze_logs(LogBuffer* buffer) {
    if (!buffer || buffer->count == 0) return;
    
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
    LogBuffer stack_buffer;
    LogEntry stack_entries[10];
    stack_buffer.entries = stack_entries;
    stack_buffer.count = 0;
    stack_buffer.capacity = 10;
    
    LogBuffer* heap_buffer = create_log_buffer(20);
    if (!heap_buffer) {
        printf("Failed to create heap buffer\n");
        return 1;
    }
    
    printf("Enter log entries (timestamp level message), one per line.\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message text\n");
    printf("Example: 2024-01-15 14:30:25 INFO System started\n");
    printf("Enter 'done' to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "done", 4) == 0) break;
        
        char timestamp[32], level[16], message[256];
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) == 3) {
            if (entry_count < 10) {
                if (add_log_entry(&stack_buffer, timestamp, level, message)) {
                    entry_count++;
                }
            } else {
                if (!add_log_entry(heap_buffer, timestamp, level, message)) {
                    printf("Failed to add log entry\n");
                }
            }
        } else {
            printf("Invalid format. Please use: YYYY-MM-DD HH:MM:SS LEVEL message\n");