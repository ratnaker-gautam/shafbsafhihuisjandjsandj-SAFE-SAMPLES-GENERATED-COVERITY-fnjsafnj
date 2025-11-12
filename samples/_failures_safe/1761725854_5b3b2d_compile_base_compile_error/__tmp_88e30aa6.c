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
    LogEntry *entries;
    int count;
    int capacity;
} LogCollection;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char level[16];
    char timestamp[32];
    char message[256];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
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

void add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry || collection->count < 0) return;
    
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity <= collection->capacity) return;
        
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
        if (line_count > 100) {
            fprintf(stderr, "Too many log entries\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (len >= MAX_LINE_LENGTH - 1) {
            fprintf(stderr, "Line too long, skipping\n");
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            add_log_entry(&logs, &entry);
        } else {
            fprintf(stderr, "Invalid log format, skipping: %s\n", line);
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
        printf("\nSample entries:\n");
        int sample_count = logs.count < 3 ? logs.count : 3;
        for (int i = 0; i < sample_count; i++) {
            printf("%s %s %s\n", logs.entries[i].timestamp, logs.entries[i].level, logs.entries[i].message);
        }
    }