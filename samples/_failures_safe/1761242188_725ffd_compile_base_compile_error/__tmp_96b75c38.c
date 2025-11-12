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
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!buffer->entries) exit(EXIT_FAILURE);
    buffer->count = 0;
    buffer->capacity = initial_capacity;
}

void free_log_buffer(LogBuffer *buffer) {
    free(buffer->entries);
    buffer->entries = NULL;
    buffer->count = 0;
    buffer->capacity = 0;
}

int resize_log_buffer(LogBuffer *buffer, int new_capacity) {
    if (new_capacity <= buffer->count) return 0;
    LogEntry *new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
    if (!new_entries) return 0;
    buffer->entries = new_entries;
    buffer->capacity = new_capacity;
    return 1;
}

int add_log_entry(LogBuffer *buffer, const char *timestamp, const char *level, const char *message) {
    if (!timestamp || !level || !message) return 0;
    if (buffer->count >= buffer->capacity) {
        if (!resize_log_buffer(buffer, buffer->capacity * 2)) return 0;
    }
    LogEntry *entry = &buffer->entries[buffer->count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    buffer->count++;
    return 1;
}

void generate_sample_logs(LogBuffer *buffer) {
    const char *levels[] = {"INFO", "WARNING", "ERROR"};
    const char *messages[] = {
        "System startup completed",
        "User login successful",
        "Disk space running low",
        "Network connection lost",
        "Backup process started",
        "Invalid input detected",
        "Service restarted",
        "Memory usage high"
    };
    time_t now = time(NULL);
    for (int i = 0; i < 20; i++) {
        char timestamp[32];
        time_t log_time = now - (i * 3600);
        struct tm *tm_info = localtime(&log_time);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        const char *level = levels[i % 3];
        const char *message = messages[i % 8];
        add_log_entry(buffer, timestamp, level, message);
    }
}

void analyze_logs(LogBuffer *buffer) {
    int info_count = 0, warning_count = 0, error_count = 0;
    for (int i = 0; i < buffer->count; i++) {
        const char *level = buffer->entries[i].level;
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARNING") == 0) warning_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
    }
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
}

void display_recent_logs(LogBuffer *buffer, int count) {
    if (count <= 0 || count > buffer->count) count = buffer->count;
    printf("Recent %d log entries:\n", count);
    for (int i = buffer->count - count; i < buffer->count; i++) {
        printf("[%s] %s: %s\n", buffer->entries[i].timestamp, 
               buffer->entries[i].level, buffer->entries[i].message);
    }
}

int main() {
    LogBuffer heap_buffer;
    LogEntry stack_entries[50];
    LogBuffer stack_buffer;
    
    stack_buffer.entries = stack_entries;
    stack_buffer.count = 0;
    stack_buffer.capacity = 50;
    
    init_log_buffer(&heap_buffer, 20);
    
    printf("Generating sample log data...\n");
    generate_sample_logs(&heap_buffer);
    
    for (int i = 0; i < heap_buffer.count && i < stack_buffer.capacity; i++) {
        add_log_entry(&stack_buffer, heap_buffer.entries[i].timestamp,
                     heap_buffer.entries[i].level, heap_buffer.entries[i].message);
    }
    
    analyze_logs(&heap_buffer);
    printf("\n");
    display_recent_logs(&stack_buffer, 5);
    
    free