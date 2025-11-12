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
    
    if (collection->count < collection->capacity) {
        collection->entries[collection->count] = *entry;
        collection->count++;
        return 1;
    }
    
    return 0;
}

void analyze_log_levels(const LogCollection *collection) {
    if (!collection) return;
    
    int level_counts[4] = {0};
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (size_t i = 0; i < collection->count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(collection->entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Level Analysis:\n");
    for (int i = 0; i < 4; i++) {
        printf("%s: %d entries\n", levels[i], level_counts[i]);
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

int main() {
    LogCollection logs;
    char input_line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    if (!init_log_collection(&logs, 10)) {
        fprintf(stderr, "Failed to initialize log collection\n");
        return 1;
    }
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(input_line, sizeof(input_line), stdin)) {
            break;
        }
        
        input_line[strcspn(input_line, "\n")] = '\0';
        
        if (strcmp(input_line, "END") == 0) {
            break;
        }
        
        if (strlen(input_line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(input_line, &entry)) {
            if (add_log_entry(&logs, &entry)) {
                entry_count++;
            } else {
                fprintf(stderr, "Failed to add log entry\n");
            }
        } else {
            fprintf(stderr, "Invalid log format: %s\n", input_line);
        }
    }
    
    if (logs.count > 0) {
        printf("\nAnalysis Results:\n");
        printf("Total entries processed: %zu\n", logs.count);
        analyze_log_levels(&logs);
    } else {
        printf("No valid log entries to analyze\n");
    }
    
    free_log_collection(&logs);
    return 0;
}