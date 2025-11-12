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

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

void init_log_buffer(LogBuffer* buffer, int initial_capacity) {
    if (!buffer || initial_capacity <= 0) return;
    
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!buffer->entries) return;
    
    buffer->count = 0;
    buffer->capacity = initial_capacity;
}

void add_log_entry(LogBuffer* buffer, const LogEntry* entry) {
    if (!buffer || !entry || buffer->count < 0) return;
    
    if (buffer->count >= buffer->capacity) {
        int new_capacity = buffer->capacity * 2;
        if (new_capacity <= buffer->capacity) return;
        
        LogEntry* new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return;
        
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    if (buffer->count < buffer->capacity) {
        buffer->entries[buffer->count] = *entry;
        buffer->count++;
    }
}

void analyze_logs(LogBuffer* buffer) {
    if (!buffer || buffer->count <= 0) return;
    
    int info_count = 0;
    int warning_count = 0;
    int error_count = 0;
    int debug_count = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        const char* level = buffer->entries[i].level;
        
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARNING") == 0) warning_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
}

void cleanup_log_buffer(LogBuffer* buffer) {
    if (buffer && buffer->entries) {
        free(buffer->entries);
        buffer->entries = NULL;
        buffer->count = 0;
        buffer->capacity = 0;
    }
}

int main() {
    LogBuffer buffer;
    init_log_buffer(&buffer, 10);
    
    if (!buffer.entries) {
        printf("Failed to initialize log buffer\n");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Maximum number of entries reached\n");
            break;
        }
        
        if (strlen(line) >= MAX_LINE_LENGTH - 1) {
            printf("Line too long, skipping\n");
            continue;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                add_log_entry(&buffer, &entry);
            } else {
                printf("Invalid log level in line: %s\n", line);
            }
        } else {
            printf("Failed to parse line: %s\n", line);
        }
    }
    
    if (buffer.count > 0) {
        analyze_logs(&buffer);
    } else {
        printf("No valid log