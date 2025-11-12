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
    LogEntry* entries;
    size_t count;
    size_t capacity;
} LogCollection;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp, line);
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void init_log_collection(LogCollection* collection, size_t initial_capacity) {
    if (!collection || initial_capacity == 0) return;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) return;
    
    collection->count = 0;
    collection->capacity = initial_capacity;
}

void add_log_entry(LogCollection* collection, const LogEntry* entry) {
    if (!collection || !entry) return;
    
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity <= collection->capacity) return;
        
        LogEntry* new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    if (collection->count < collection->capacity) {
        collection->entries[collection->count] = *entry;
        collection->count++;
    }
}

void count_log_levels(const LogCollection* collection, int* counts) {
    if (!collection || !counts) return;
    
    for (size_t i = 0; i < collection->count; i++) {
        const char* level = collection->entries[i].level;
        
        if (strcmp(level, "INFO") == 0) counts[0]++;
        else if (strcmp(level, "WARNING") == 0) counts[1]++;
        else if (strcmp(level, "ERROR") == 0) counts[2]++;
        else counts[3]++;
    }
}

void cleanup_log_collection(LogCollection* collection) {
    if (collection && collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
        collection->count = 0;
        collection->capacity = 0;
    }
}

int main() {
    LogCollection logs;
    init_log_collection(&logs, 10);
    
    char input[MAX_LINE_LENGTH];
    printf("Enter log entries (empty line to finish):\n");
    
    while (fgets(input, sizeof(input), stdin)) {
        if (input[0] == '\n') break;
        
        input[strcspn(input, "\n")] = 0;
        
        if (strlen(input) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(input, &entry)) {
            add_log_entry(&logs, &entry);
        } else {
            printf("Invalid log format: %s\n", input);
        }
    }
    
    if (logs.count == 0) {
        printf("No valid log entries found.\n");
        cleanup_log_collection(&logs);
        return 1;
    }
    
    int level_counts[4] = {0};
    count_log_levels(&logs, level_counts);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %zu\n", logs.count);
    printf("INFO: %d\n", level_counts[0]);
    printf("WARNING: %d\n", level_counts[1]);
    printf("ERROR: %d\n", level_counts[2]);
    printf("OTHER: %d\n", level_counts[3]);
    
    printf("\nRecent entries:\n");
    size_t display_count = (logs.count > 5) ? 5 : logs.count;
    for (size_t i = logs.count - display_count; i < logs.count; i++) {
        printf("%s %s %s\n", logs.entries[i].timestamp, logs.entries[i].level, logs.entries[i].message);
    }
    
    cleanup_log_collection(&logs);
    return 0;
}