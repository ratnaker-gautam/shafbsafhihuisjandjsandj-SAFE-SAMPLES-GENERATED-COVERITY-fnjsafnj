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

LogCollection *create_log_collection(size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > 10000) return NULL;
    
    LogCollection *collection = malloc(sizeof(LogCollection));
    if (!collection) return NULL;
    
    collection->entries = malloc(sizeof(LogEntry) * initial_capacity);
    if (!collection->entries) {
        free(collection);
        return NULL;
    }
    
    collection->count = 0;
    collection->capacity = initial_capacity;
    return collection;
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry) return 0;
    
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity > 10000) return 0;
        
        LogEntry *new_entries = realloc(collection->entries, sizeof(LogEntry) * new_capacity);
        if (!new_entries) return 0;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    collection->entries[collection->count] = *entry;
    collection->count++;
    return 1;
}

void free_log_collection(LogCollection *collection) {
    if (collection) {
        free(collection->entries);
        free(collection);
    }
}

void analyze_log_levels(const LogCollection *collection) {
    if (!collection) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < collection->count; i++) {
        const char *level = collection->entries[i].level;
        if (strcmp(level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Level Analysis:\n");
    printf("  INFO: %d entries\n", info_count);
    printf("  WARNING: %d entries\n", warning_count);
    printf("  ERROR: %d entries\n", error_count);
}

int main() {
    LogCollection *collection = create_log_collection(10);
    if (!collection) {
        fprintf(stderr, "Failed to create log collection\n");
        return 1;
    }
    
    char input_buffer[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        if (strncmp(input_buffer, "END", 3) == 0) {
            break;
        }
        
        line_count++;
        if (line_count > 50) {
            printf("Maximum input lines reached\n");
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len-1] == '\n') {
            input_buffer[len-1] = '\0';
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(input_buffer, &entry)) {
            if (!add_log_entry(collection, &entry)) {
                fprintf(stderr, "Failed to add log entry\n");
                break;
            }
        } else {
            fprintf(stderr, "Failed to parse line: %s\n", input_buffer);
        }
    }
    
    if (collection->count > 0) {
        printf("\nAnalysis Results:\n");
        printf("Total log entries: %zu\n", collection->count);
        analyze_log_levels(collection);
        
        printf("\nFirst 5 entries:\n");
        size_t display_count = collection->count > 5 ? 5 : collection->count;
        for (size_t i = 0; i < display_count; i