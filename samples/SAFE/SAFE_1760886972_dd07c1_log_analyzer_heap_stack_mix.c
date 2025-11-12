//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t count;
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t size;
    size_t capacity;
} LogCollection;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
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
    entry->count = 1;
    
    return 1;
}

void init_log_collection(LogCollection *collection, size_t initial_capacity) {
    if (!collection || initial_capacity == 0) return;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) return;
    
    collection->size = 0;
    collection->capacity = initial_capacity;
}

void add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry) return;
    
    if (collection->size >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity <= collection->capacity) return;
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    collection->entries[collection->size] = *entry;
    collection->size++;
}

void free_log_collection(LogCollection *collection) {
    if (!collection) return;
    free(collection->entries);
    collection->entries = NULL;
    collection->size = 0;
    collection->capacity = 0;
}

void analyze_logs(LogCollection *collection) {
    if (!collection || collection->size == 0) return;
    
    uint32_t error_count = 0;
    uint32_t warning_count = 0;
    uint32_t info_count = 0;
    
    for (size_t i = 0; i < collection->size; i++) {
        if (strcmp(collection->entries[i].level, "ERROR") == 0) {
            error_count += collection->entries[i].count;
        } else if (strcmp(collection->entries[i].level, "WARNING") == 0) {
            warning_count += collection->entries[i].count;
        } else if (strcmp(collection->entries[i].level, "INFO") == 0) {
            info_count += collection->entries[i].count;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", collection->size);
    printf("ERROR count: %u\n", error_count);
    printf("WARNING count: %u\n", warning_count);
    printf("INFO count: %u\n", info_count);
}

int main() {
    LogCollection collection;
    init_log_collection(&collection, 10);
    
    if (!collection.entries) {
        printf("Failed to initialize log collection\n");
        return 1;
    }
    
    char input_buffer[MAX_LINE_LEN];
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE), empty line to finish:\n");
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        if (input_buffer[0] == '\n') break;
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len-1] == '\n') {
            input_buffer[len-1] = '\0';
        }
        
        if (strlen(input_buffer) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(input_buffer, &entry)) {
            add_log_entry(&collection, &entry);
        } else {
            printf("Invalid log format: %s\n", input_buffer);
        }
    }
    
    if (collection.size > 0) {
        analyze_logs(&collection);
    } else {
        printf("No valid log entries to analyze\n");
    }
    
    free_log_collection(&collection);
    return 0;
}