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
    int count;
    int capacity;
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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void init_log_collection(LogCollection *collection, int initial_capacity) {
    if (!collection || initial_capacity <= 0) return;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) return;
    
    collection->count = 0;
    collection->capacity = initial_capacity;
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry) return 0;
    
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    if (collection->count >= collection->capacity) return 0;
    
    collection->entries[collection->count] = *entry;
    collection->count++;
    return 1;
}

void count_log_levels(const LogCollection *collection, int *error_count, int *warning_count, int *info_count) {
    if (!collection || !error_count || !warning_count || !info_count) return;
    
    *error_count = 0;
    *warning_count = 0;
    *info_count = 0;
    
    for (int i = 0; i < collection->count; i++) {
        if (strcmp(collection->entries[i].level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(collection->entries[i].level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(collection->entries[i].level, "INFO") == 0) {
            (*info_count)++;
        }
    }
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
    LogCollection logs;
    init_log_collection(&logs, 10);
    
    if (!logs.entries) {
        fprintf(stderr, "Failed to initialize log collection\n");
        return 1;
    }
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        
        if (line_count > MAX_ENTRIES) {
            printf("Maximum number of entries reached\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (!add_log_entry(&logs, &entry)) {
                printf("Failed to add log entry\n");
                break;
            }
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", logs.count);
    
    int error_count, warning_count, info_count;
    count_log_levels(&logs, &error_count, &warning_count, &info_count);
    
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (logs.count > 0) {
        printf("\nFirst 5 entries:\n");
        int display_count = logs.count > 5