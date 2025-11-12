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
    LogEntry *entries;
    int count;
    int capacity;
} LogBuffer;

void init_log_buffer(LogBuffer *buffer, int initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = 10;
    if (initial_capacity > MAX_ENTRIES) initial_capacity = MAX_ENTRIES;
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!buffer->entries) exit(1);
    buffer->count = 0;
    buffer->capacity = initial_capacity;
}

void add_log_entry(LogBuffer *buffer, const char *timestamp, const char *level, const char *message) {
    if (!buffer || !timestamp || !level || !message) return;
    
    if (buffer->count >= buffer->capacity) {
        int new_capacity = buffer->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        if (new_capacity <= buffer->capacity) return;
        LogEntry *new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return;
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    if (buffer->count < buffer->capacity) {
        LogEntry *entry = &buffer->entries[buffer->count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        buffer->count++;
    }
}

void free_log_buffer(LogBuffer *buffer) {
    if (buffer) {
        free(buffer->entries);
        buffer->entries = NULL;
        buffer->count = 0;
        buffer->capacity = 0;
    }
}

void generate_sample_logs(LogBuffer *buffer) {
    if (!buffer) return;
    
    char timestamp[32];
    time_t now = time(NULL);
    if (now == (time_t)-1) return;
    
    struct tm tm_info;
    if (!localtime_r(&now, &tm_info)) return;
    
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_info);
    add_log_entry(buffer, timestamp, "INFO", "System started successfully");
    
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_info);
    add_log_entry(buffer, timestamp, "WARNING", "High memory usage detected");
    
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_info);
    add_log_entry(buffer, timestamp, "ERROR", "Failed to connect to database");
    
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_info);
    add_log_entry(buffer, timestamp, "INFO", "User login successful");
    
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_info);
    add_log_entry(buffer, timestamp, "DEBUG", "Processing request ID 12345");
}

void analyze_logs(LogBuffer *buffer) {
    if (!buffer) return;
    
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        const LogEntry *entry = &buffer->entries[i];
        
        if (strcmp(entry->level, "INFO") == 0) info_count++;
        else if (strcmp(entry->level, "WARNING") == 0) warning_count++;
        else if (strcmp(entry->level, "ERROR") == 0) error_count++;
        else if (strcmp(entry->level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (error_count > 0) {
        printf("\nError entries:\n");
        for (int i = 0; i < buffer->count; i++) {
            const LogEntry *entry = &buffer->entries[i];
            if (strcmp(entry->level, "ERROR") == 0) {
                printf("- %s: %s\n", entry->timestamp, entry->message);
            }
        }
    }
}

int main() {
    LogBuffer heap_buffer;
    LogEntry stack_entries[5];
    LogBuffer stack_buffer