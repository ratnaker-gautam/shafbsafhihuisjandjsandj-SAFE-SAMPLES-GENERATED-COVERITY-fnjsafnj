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
} LogEntry;

typedef struct {
    LogEntry* entries;
    size_t count;
    size_t capacity;
} LogBuffer;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void init_log_buffer(LogBuffer* buffer, size_t initial_capacity) {
    if (!buffer || initial_capacity == 0) return;
    
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!buffer->entries) return;
    
    buffer->count = 0;
    buffer->capacity = initial_capacity;
}

void add_to_buffer(LogBuffer* buffer, const LogEntry* entry) {
    if (!buffer || !entry || buffer->count >= buffer->capacity) return;
    
    buffer->entries[buffer->count] = *entry;
    buffer->count++;
}

void analyze_logs(LogBuffer* buffer) {
    if (!buffer || buffer->count == 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < buffer->count; i++) {
        if (strcmp(buffer->entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(buffer->entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(buffer->entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", buffer->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        int printed = 0;
        for (size_t i = buffer->count - 1; i >= 0 && printed < 3; i--) {
            if (strcmp(buffer->entries[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", buffer->entries[i].timestamp, buffer->entries[i].message);
                printed++;
            }
        }
    }
}

int main() {
    LogBuffer buffer;
    init_log_buffer(&buffer, 100);
    
    if (!buffer.entries) {
        fprintf(stderr, "Failed to initialize log buffer\n");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        
        if (line_count > MAX_ENTRIES) {
            printf("Maximum entry limit reached\n");
            break;
        }
        
        if (strlen(line) > 0 && line[strlen(line)-1] == '\n') {
            line[strlen(line)-1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            add_to_buffer(&buffer, &entry);
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (buffer.count > 0) {
        analyze_logs(&buffer);
    } else {
        printf("No valid log entries to analyze\n");
    }
    
    free(buffer.entries);
    return 0;
}