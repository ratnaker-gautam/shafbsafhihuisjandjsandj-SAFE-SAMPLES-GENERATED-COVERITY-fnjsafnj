//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) return 0;
    strcpy(entry->timestamp, timestamp);
    
    if (strlen(level) >= sizeof(entry->level) - 1) return 0;
    strcpy(entry->level, level);
    
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    strcpy(entry->message, message);
    
    entry->line_number = line_num;
    return 1;
}

int init_log_collection(LogCollection *collection, size_t initial_capacity) {
    if (!collection || initial_capacity == 0) return 0;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) return 0;
    
    collection->count = 0;
    collection->capacity = initial_capacity;
    return 1;
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry) return 0;
    
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity <= collection->capacity) return 0;
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    collection->entries[collection->count] = *entry;
    collection->count++;
    return 1;
}

void free_log_collection(LogCollection *collection) {
    if (collection && collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
        collection->count = 0;
        collection->capacity = 0;
    }
}

void analyze_logs(const LogCollection *collection) {
    if (!collection) return;
    
    size_t error_count = 0;
    size_t warning_count = 0;
    size_t info_count = 0;
    
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
    printf("ERROR entries: %zu\n", error_count);
    printf("WARNING entries: %zu\n", warning_count);
    printf("INFO entries: %zu\n", info_count);
    
    if (collection->count > 0) {
        printf("First timestamp: %s\n", collection->entries[0].timestamp);
        printf("Last timestamp: %s\n", collection->entries[collection->count - 1].timestamp);
    }
}

int main() {
    LogCollection collection;
    if (!init_log_collection(&collection, 10)) {
        fprintf(stderr, "Failed to initialize log collection\n");
        return 1;
    }
    
    char input_buffer[MAX_LINE_LENGTH];
    uint32_t line_number = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        line_number++;
        
        if (strlen(input_buffer) > 0 && input_buffer[strlen(input_buffer) - 1] == '\n') {
            input_buffer[strlen(input_buffer) - 1] = '\0';
        }
        
        if (strcmp(input_buffer, "END") == 0) {
            break;
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(input_buffer, &entry, line_number)) {
            if (!add_log_entry(&collection, &entry)) {
                fprintf(stderr, "Failed to add log entry\n");
                free_log_collection(&collection);
                return