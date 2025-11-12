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

int parse_timestamp(const char* ts_str, struct tm* tm) {
    return sscanf(ts_str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 ||
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

LogBuffer* create_buffer(int initial_capacity) {
    if (initial_capacity <= 0 || initial_capacity > 10000) return NULL;
    
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

void destroy_buffer(LogBuffer* buffer) {
    if (buffer) {
        free(buffer->entries);
        free(buffer);
    }
}

int add_entry(LogBuffer* buffer, const char* timestamp, const char* level, const char* message) {
    if (!buffer || !timestamp || !level || !message) return 0;
    if (buffer->count >= buffer->capacity) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    struct tm tm;
    if (!parse_timestamp(timestamp, &tm)) return 0;
    if (!is_valid_level(level)) return 0;
    
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

int main(void) {
    LogBuffer* buffer = create_buffer(50);
    if (!buffer) {
        fprintf(stderr, "Failed to create log buffer\n");
        return 1;
    }
    
    LogEntry sample_logs[] = {
        {"2024-01-15 10:30:00", "INFO", "System started successfully"},
        {"2024-01-15 10:31:15", "DEBUG", "Initializing modules"},
        {"2024-01-15 10:32:30", "WARN", "High memory usage detected"},
        {"2024-01-15 10:33:45", "ERROR", "Database connection failed"},
        {"2024-01-15 10:34:00", "INFO", "Retrying connection"},
        {"2024-01-15 10:35:20", "INFO", "Connection established"}
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count; i++) {
        if (!add_entry(buffer, sample_logs[i].timestamp, 
                      sample_logs[i].level, sample_logs[i].message)) {
            fprintf(stderr, "Failed to add log entry %d\n", i);
            destroy_buffer(buffer);
            return 1;
        }
    }
    
    analyze_logs(buffer);
    destroy_buffer(buffer);
    
    return 0;
}