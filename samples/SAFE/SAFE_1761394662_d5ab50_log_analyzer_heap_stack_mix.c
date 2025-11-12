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
    
    char level[16] = {0};
    char timestamp[32] = {0};
    char message[256] = {0};
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry) return 0;
    
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity <= 0) new_capacity = 10;
        
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

void count_log_levels(const LogCollection *collection) {
    if (!collection) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < collection->count; i++) {
        const char *level = collection->entries[i].level;
        if (strcmp(level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Level Summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
}

void display_recent_logs(const LogCollection *collection, int count) {
    if (!collection || count <= 0) return;
    
    if (count > collection->count) {
        count = collection->count;
    }
    
    printf("Recent %d log entries:\n", count);
    for (int i = collection->count - count; i < collection->count; i++) {
        printf("%s %s %s\n", collection->entries[i].timestamp, 
               collection->entries[i].level, collection->entries[i].message);
    }
}

int main(void) {
    LogCollection collection = {0};
    collection.capacity = 10;
    collection.entries = malloc(collection.capacity * sizeof(LogEntry));
    if (!collection.entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (line_count >= MAX_ENTRIES) {
            printf("Maximum entries reached\n");
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (add_log_entry(&collection, &entry)) {
                line_count++;
            } else {
                fprintf(stderr, "Failed to add log entry\n");
            }
        } else {
            fprintf(stderr, "Invalid log format: %s\n", line);
        }
    }
    
    if (collection.count > 0) {
        printf("\nAnalysis Results:\n");
        count_log_levels(&collection);
        printf("\n");
        display_recent_logs(&collection, 3);
    } else {
        printf("No valid log entries to analyze\n");
    }
    
    free(collection.entries);
    return 0;
}