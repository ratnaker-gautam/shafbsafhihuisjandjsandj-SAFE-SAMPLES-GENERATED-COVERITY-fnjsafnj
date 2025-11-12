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

void analyze_log_levels(const LogCollection *collection) {
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
    
    printf("Log Level Analysis:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
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
    
    char input_line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (fgets(input_line, sizeof(input_line), stdin)) {
        if (line_count >= MAX_ENTRIES) {
            printf("Maximum log entries reached.\n");
            break;
        }
        
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        if (strcmp(input_line, "END") == 0) {
            break;
        }
        
        if (strlen(input_line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(input_line, &entry)) {
            add_log_entry(&collection, &entry);
            line_count++;
        } else {
            printf("Invalid log format: %s\n", input_line);
        }
    }
    
    if (collection.count > 0) {
        printf("\nAnalysis Results:\n");
        printf("Total log entries: %zu\n", collection.count);
        analyze_log_levels(&collection);
        
        printf("\nSample entries:\n");
        size_t sample_count = collection.count > 3 ? 3 : collection.count;
        for (size_t i = 0; i < sample_count; i++) {
            const LogEntry *entry = &collection.entries[i];
            printf("%s %s %s\n", entry->timestamp, entry->level, entry->message);
        }
    } else {
        printf("No valid log entries to analyze.\n");