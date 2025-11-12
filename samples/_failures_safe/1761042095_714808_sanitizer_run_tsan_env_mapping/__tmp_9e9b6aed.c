//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t line_number;
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t count;
    size_t capacity;
} LogCollection;

int parse_log_line(const char *line, LogEntry *entry, uint32_t line_num) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->line_number = line_num;
    
    return 1;
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry) return 0;
    
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity == 0) new_capacity = 16;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    if (collection->count < collection->capacity) {
        collection->entries[collection->count] = *entry;
        collection->count++;
        return 1;
    }
    
    return 0;
}

void analyze_logs(const LogCollection *collection) {
    if (!collection || collection->count == 0) {
        printf("No logs to analyze.\n");
        return;
    }
    
    uint32_t error_count = 0;
    uint32_t warning_count = 0;
    uint32_t info_count = 0;
    
    for (size_t i = 0; i < collection->count; i++) {
        const LogEntry *entry = &collection->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", collection->count);
    printf("ERROR entries: %u\n", error_count);
    printf("WARNING entries: %u\n", warning_count);
    printf("INFO entries: %u\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        for (size_t i = 0; i < collection->count && i < 5; i++) {
            const LogEntry *entry = &collection->entries[i];
            if (strcmp(entry->level, "ERROR") == 0) {
                printf("Line %u: %s - %s\n", entry->line_number, entry->timestamp, entry->message);
            }
        }
    }
}

int main(void) {
    LogCollection collection = {0};
    char input_buffer[MAX_LINE_LENGTH];
    uint32_t line_number = 0;
    
    printf("Enter log entries (empty line to finish):\n");
    printf("Format: TIMESTAMP LEVEL MESSAGE\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (strlen(input_buffer) == 0) {
            break;
        }
        
        line_number++;
        LogEntry entry;
        
        if (parse_log_line(input_buffer, &entry, line_number)) {
            if (!add_log_entry(&collection, &entry)) {
                printf("Failed to add log entry at line %u\n", line_number);
            }
        } else {
            printf("Failed to parse line %u: %s\n", line_number, input_buffer);
        }
        
        if (collection.count >= MAX_ENTRIES) {
            printf("Maximum log entries reached.\n");
            break;
        }
    }
    
    analyze_logs(&collection);
    
    if (collection.entries) {
        free(collection.entries);
    }
    
    return 0;
}