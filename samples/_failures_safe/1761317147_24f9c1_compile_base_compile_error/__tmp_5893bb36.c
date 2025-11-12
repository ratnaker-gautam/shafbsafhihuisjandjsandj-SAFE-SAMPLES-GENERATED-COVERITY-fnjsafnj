//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t count;
    size_t capacity;
} LogCollection;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void init_log_collection(LogCollection *collection) {
    if (!collection) return;
    collection->entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (collection->entries) {
        collection->capacity = MAX_ENTRIES;
        collection->count = 0;
    } else {
        collection->capacity = 0;
        collection->count = 0;
    }
}

void free_log_collection(LogCollection *collection) {
    if (collection && collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
        collection->count = 0;
        collection->capacity = 0;
    }
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry || !collection->entries) return 0;
    if (collection->count >= collection->capacity) return 0;
    
    if (strlen(entry->timestamp) >= sizeof(collection->entries[0].timestamp)) return 0;
    if (strlen(entry->level) >= sizeof(collection->entries[0].level)) return 0;
    if (strlen(entry->message) >= sizeof(collection->entries[0].message)) return 0;
    
    strcpy(collection->entries[collection->count].timestamp, entry->timestamp);
    strcpy(collection->entries[collection->count].level, entry->level);
    strcpy(collection->entries[collection->count].message, entry->message);
    collection->count++;
    
    return 1;
}

void analyze_logs(const LogCollection *collection) {
    if (!collection || !collection->entries) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
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
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (collection->count > 0) {
        printf("First timestamp: %s\n", collection->entries[0].timestamp);
        printf("Last timestamp: %s\n", collection->entries[collection->count - 1].timestamp);
    }
}

int main() {
    LogCollection collection;
    init_log_collection(&collection);
    
    if (!collection.entries) {
        printf("Failed to initialize log collection\n");
        return 1;
    }
    
    char input_buffer[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        if (line_count >= MAX_ENTRIES) {
            printf("Maximum log entries reached\n");
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (strcmp(input_buffer, "END") == 0) {
            break;
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(input_buffer, &entry)) {
            if (add_log_entry(&collection, &entry)) {