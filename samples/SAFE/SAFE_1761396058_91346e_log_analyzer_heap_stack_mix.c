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

void init_buffer(LogBuffer *buffer, int initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = 10;
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!buffer->entries) exit(EXIT_FAILURE);
    buffer->count = 0;
    buffer->capacity = initial_capacity;
}

void add_entry(LogBuffer *buffer, const char *timestamp, const char *level, const char *message) {
    if (buffer->count >= buffer->capacity) {
        int new_capacity = buffer->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        if (buffer->count >= new_capacity) return;
        
        LogEntry *new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return;
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    if (timestamp) strncpy(buffer->entries[buffer->count].timestamp, timestamp, 31);
    else buffer->entries[buffer->count].timestamp[0] = '\0';
    
    if (level) strncpy(buffer->entries[buffer->count].level, level, 15);
    else buffer->entries[buffer->count].level[0] = '\0';
    
    if (message) strncpy(buffer->entries[buffer->count].message, message, 255);
    else buffer->entries[buffer->count].message[0] = '\0';
    
    buffer->entries[buffer->count].timestamp[31] = '\0';
    buffer->entries[buffer->count].level[15] = '\0';
    buffer->entries[buffer->count].message[255] = '\0';
    
    buffer->count++;
}

void free_buffer(LogBuffer *buffer) {
    if (buffer->entries) {
        free(buffer->entries);
        buffer->entries = NULL;
    }
    buffer->count = 0;
    buffer->capacity = 0;
}

void analyze_logs(LogBuffer *buffer) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        if (strcmp(buffer->entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(buffer->entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(buffer->entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        for (int i = buffer->count - 1; i >= 0 && i >= buffer->count - 5; i--) {
            if (strcmp(buffer->entries[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", buffer->entries[i].timestamp, buffer->entries[i].message);
            }
        }
    }
}

int main(void) {
    LogBuffer buffer;
    init_buffer(&buffer, 20);
    
    char line[MAX_LINE_LENGTH];
    char timestamp[32];
    char level[16];
    char message[256];
    
    printf("Enter log entries (format: timestamp level message), or 'quit' to finish:\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "quit", 4) == 0) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed >= 2) {
            if (parsed == 2) {
                message[0] = '\0';
            }
            add_entry(&buffer, timestamp, level, message);
        } else {
            printf("Invalid format. Use: timestamp level message\n");
        }
        
        if (buffer.count >= MAX_ENTRIES) {
            printf("Maximum entries reached.\n");
            break;
        }
    }
    
    if (buffer.count > 0) {
        analyze_logs(&buffer);
    } else {
        printf("No log entries to analyze.\n");
    }
    
    free_buffer(&buffer);
    return 0;
}