//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef struct {
    LogEntry* entries;
    int count;
    int capacity;
} LogCollection;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

void init_log_collection(LogCollection* collection, int initial_capacity) {
    if (!collection || initial_capacity <= 0) return;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) return;
    
    collection->count = 0;
    collection->capacity = initial_capacity;
}

void free_log_collection(LogCollection* collection) {
    if (!collection) return;
    free(collection->entries);
    collection->entries = NULL;
    collection->count = 0;
    collection->capacity = 0;
}

int add_log_entry(LogCollection* collection, const LogEntry* entry) {
    if (!collection || !entry) return 0;
    
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity <= collection->capacity) return 0;
        
        LogEntry* new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
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

void analyze_logs(const LogCollection* collection) {
    if (!collection) return;
    
    int level_counts[4] = {0};
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    
    for (int i = 0; i < collection->count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(collection->entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", collection->count);
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
}

int main() {
    char buffer[MAX_LINE_LENGTH];
    LogCollection collection;
    
    init_log_collection(&collection, 10);
    if (!collection.entries) {
        fprintf(stderr, "Failed to initialize log collection\n");
        return 1;
    }
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR, DEBUG\n");
    printf("Enter 'END' on a separate line to finish\n");
    
    while (fgets(buffer, sizeof(buffer), stdin)) {
        if (strlen(buffer) > 0 && buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
        }
        
        if (strcmp(buffer, "END") == 0) break;
        
        if (strlen(buffer) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(buffer, &entry)) {
            if (is_valid_level(entry.level)) {
                if (!add_log_entry(&collection, &entry)) {
                    fprintf(stderr, "Failed to add log entry\n");
                }
            } else {
                fprintf(stderr, "Invalid log level: %s\n", entry.level);
            }
        } else {
            fprintf(stderr, "Invalid log format\n");
        }
    }
    
    if (collection.count > 0) {
        analyze_logs(&collection);
    } else {
        printf("No valid log entries to analyze\n");
    }
    
    free_log_collection(&collection);
    return 0;
}