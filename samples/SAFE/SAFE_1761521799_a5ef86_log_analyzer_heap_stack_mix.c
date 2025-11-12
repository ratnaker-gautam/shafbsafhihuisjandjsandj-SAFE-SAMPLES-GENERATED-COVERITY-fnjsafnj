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
    LogEntry *entries;
    size_t count;
    size_t capacity;
} LogCollection;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(buffer, line);
    
    char *timestamp = strtok(buffer, " ");
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

void init_log_collection(LogCollection *collection, size_t initial_capacity) {
    if (!collection || initial_capacity == 0) return;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) return;
    
    collection->count = 0;
    collection->capacity = initial_capacity;
}

void add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry) return;
    
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    if (collection->count < collection->capacity) {
        collection->entries[collection->count] = *entry;
        collection->count++;
    }
}

void analyze_logs(const LogCollection *collection) {
    if (!collection) return;
    
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
}

void cleanup_log_collection(LogCollection *collection) {
    if (collection && collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
        collection->count = 0;
        collection->capacity = 0;
    }
}

int main() {
    LogCollection collection;
    init_log_collection(&collection, 10);
    
    if (!collection.entries) {
        fprintf(stderr, "Failed to initialize log collection\n");
        return 1;
    }
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin)) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            add_log_entry(&collection, &entry);
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (collection.count > 0) {
        analyze_logs(&collection);
    } else {
        printf("No valid log entries to analyze\n");
    }
    
    cleanup_log_collection(&collection);
    return 0;
}