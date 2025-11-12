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

LogBuffer* create_log_buffer(int initial_capacity) {
    if (initial_capacity <= 0) {
        return NULL;
    }
    
    LogBuffer* buffer = malloc(sizeof(LogBuffer));
    if (!buffer) {
        return NULL;
    }
    
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
    if (!buffer || !timestamp || !level || !message) {
        return 0;
    }
    
    if (buffer->count >= buffer->capacity) {
        int new_capacity = buffer->capacity * 2;
        if (new_capacity > MAX_ENTRIES) {
            return 0;
        }
        
        LogEntry* new_entries = realloc(buffer->entries, sizeof(LogEntry) * new_capacity);
        if (!new_entries) {
            return 0;
        }
        
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

void analyze_log_levels(LogBuffer* buffer) {
    if (!buffer || buffer->count == 0) {
        return;
    }
    
    int info_count = 0;
    int warning_count = 0;
    int error_count = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        const char* level = buffer->entries[i].level;
        if (strcmp(level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(level, "ERROR") == 0) {
            error_count++;
        }
    }
    
    printf("Log Level Analysis:\n");
    printf("INFO: %d entries\n", info_count);
    printf("WARNING: %d entries\n", warning_count);
    printf("ERROR: %d entries\n", error_count);
    printf("Total: %d entries\n", buffer->count);
}

int main() {
    LogBuffer* buffer = create_log_buffer(10);
    if (!buffer) {
        printf("Failed to create log buffer\n");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    printf("Enter log entries (format: timestamp level message), empty line to finish:\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        char timestamp[32];
        char level[16];
        char message[256];
        
        int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (result == 3) {
            if (!add_log_entry(buffer, timestamp, level, message)) {
                printf("Failed to add log entry\n");
                break;
            }
        } else {
            printf("Invalid format. Use: timestamp level message\n");
        }
        
        if (buffer->count >= MAX_ENTRIES) {
            printf("Maximum entries reached\n");
            break;
        }
    }
    
    if (buffer->count > 0) {
        analyze_log_levels(buffer);
        
        printf("\nRecent entries:\n");
        int start = buffer->count > 5 ? buffer->count - 5 : 0;
        for (int i = start; i < buffer->count; i++) {
            printf("%s %s: %s\n", buffer->entries[i].timestamp, 
                   buffer->entries[i].level, buffer->entries[i].message);
        }
    } else {
        printf("No log entries to analyze\n");
    }
    
    destroy_log_buffer(buffer);
    return 0;
}