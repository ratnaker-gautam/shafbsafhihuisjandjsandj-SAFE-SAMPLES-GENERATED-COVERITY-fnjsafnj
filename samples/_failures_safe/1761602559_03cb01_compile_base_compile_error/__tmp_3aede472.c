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
    LogEntry* entries;
    size_t count;
    size_t capacity;
} LogCollection;

int validate_log_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG", "CRITICAL"};
    for (size_t i = 0; i < 5; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int parse_log_line(const char* line, LogEntry* entry, uint32_t line_num) {
    if (!line || !entry || strlen(line) == 0) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    if (!validate_log_level(level)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->line_number = line_num;
    
    return 1;
}

LogCollection* create_log_collection(size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > 10000) return NULL;
    
    LogCollection* collection = malloc(sizeof(LogCollection));
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

int add_log_entry(LogCollection* collection, const LogEntry* entry) {
    if (!collection || !entry) return 0;
    
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity > 10000) return 0;
        
        LogEntry* new_entries = realloc(collection->entries, sizeof(LogEntry) * new_capacity);
        if (!new_entries) return 0;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    collection->entries[collection->count] = *entry;
    collection->count++;
    return 1;
}

void free_log_collection(LogCollection* collection) {
    if (collection) {
        free(collection->entries);
        free(collection);
    }
}

void analyze_logs(const LogCollection* collection) {
    if (!collection || collection->count == 0) return;
    
    size_t level_counts[5] = {0};
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG", "CRITICAL"};
    
    for (size_t i = 0; i < collection->count; i++) {
        for (size_t j = 0; j < 5; j++) {
            if (strcmp(collection->entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", collection->count);
    for (size_t i = 0; i < 5; i++) {
        printf("%s: %zu\n", levels[i], level_counts[i]);
    }
}

int main() {
    LogCollection* collection = create_log_collection(10);
    if (!collection) {
        printf("Failed to create log collection\n");
        return 1;
    }
    
    char input_buffer[MAX_LINE_LENGTH];
    uint32_t line_number = 1;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR, DEBUG, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (fgets(input_buffer, MAX_LINE_LENGTH, stdin)) {
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
        if (parse_log_line(input_buffer, &entry, line_number)) {
            if (!add_log_entry(collection, &entry)) {
                printf("Failed to add log entry\n");
                break;
            }
        } else {
            printf("Invalid log format at line %u\n", line_number);
        }
        line_number++;