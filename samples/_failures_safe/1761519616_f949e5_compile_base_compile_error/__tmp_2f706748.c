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
    uint32_t line_number;
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t count;
    size_t capacity;
} LogCollection;

int validate_log_level(const char *level) {
    const char *valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG", "CRITICAL"};
    for (size_t i = 0; i < 5; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int parse_log_line(const char *line, LogEntry *entry, uint32_t line_num) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 31 || strlen(level) >= 15 || strlen(message) >= 255) return 0;
    if (!validate_log_level(level)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->line_number = line_num;
    
    return 1;
}

LogCollection *create_log_collection(size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_ENTRIES) return NULL;
    
    LogCollection *collection = malloc(sizeof(LogCollection));
    if (!collection) return NULL;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
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
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        if (new_capacity <= collection->capacity) return 0;
        
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

void free_log_collection(LogCollection *collection) {
    if (collection) {
        free(collection->entries);
        free(collection);
    }
}

void analyze_log_levels(const LogCollection *collection) {
    if (!collection) return;
    
    size_t level_counts[5] = {0};
    const char *levels[] = {"INFO", "WARNING", "ERROR", "DEBUG", "CRITICAL"};
    
    for (size_t i = 0; i < collection->count; i++) {
        for (size_t j = 0; j < 5; j++) {
            if (strcmp(collection->entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Level Analysis:\n");
    for (size_t i = 0; i < 5; i++) {
        printf("%s: %zu entries\n", levels[i], level_counts[i]);
    }
}

int main() {
    char input_buffer[MAX_LINE_LENGTH];
    LogCollection *collection = create_log_collection(10);
    if (!collection) {
        printf("Failed to create log collection\n");
        return 1;
    }
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR, DEBUG, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    uint32_t line_number = 1;
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        if (strlen(input_buffer) > 0 && input_buffer[strlen(input_buffer) - 1] == '\n') {
            input_buffer[strlen(input_buffer) - 1] = '\0';
        }
        
        if (strcmp(input_buffer, "END") == 0) {
            break;
        }
        
        if (strlen(input_buffer) == 0) {
            line_number++;
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(input_buffer, &entry, line_number)) {
            if (!add_log_entry(collection, &entry)) {
                printf("Failed to add log entry at