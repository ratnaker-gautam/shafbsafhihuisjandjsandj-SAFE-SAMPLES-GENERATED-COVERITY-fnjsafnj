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

int init_log_collection(LogCollection *collection, int initial_capacity) {
    if (!collection || initial_capacity <= 0) return 0;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) return 0;
    
    collection->count = 0;
    collection->capacity = initial_capacity;
    return 1;
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry) return 0;
    
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
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

void free_log_collection(LogCollection *collection) {
    if (collection && collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
        collection->count = 0;
        collection->capacity = 0;
    }
}

int count_logs_by_level(const LogCollection *collection, const char *level) {
    if (!collection || !level) return 0;
    
    int count = 0;
    for (int i = 0; i < collection->count; i++) {
        if (strcmp(collection->entries[i].level, level) == 0) {
            count++;
        }
    }
    return count;
}

void print_log_summary(const LogCollection *collection) {
    if (!collection) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", collection->count);
    printf("INFO logs: %d\n", count_logs_by_level(collection, "INFO"));
    printf("WARNING logs: %d\n", count_logs_by_level(collection, "WARNING"));
    printf("ERROR logs: %d\n", count_logs_by_level(collection, "ERROR"));
    printf("DEBUG logs: %d\n", count_logs_by_level(collection, "DEBUG"));
    
    if (collection->count > 0) {
        printf("\nFirst entry: %s %s %s\n", 
               collection->entries[0].timestamp,
               collection->entries[0].level,
               collection->entries[0].message);
        printf("Last entry: %s %s %s\n", 
               collection->entries[collection->count-1].timestamp,
               collection->entries[collection->count-1].level,
               collection->entries[collection->count-1].message);
    }
}

int main() {
    LogCollection logs;
    if (!init_log_collection(&logs, 10)) {
        fprintf(stderr, "Failed to initialize log collection\n");
        return 1;
    }
    
    char input_line[MAX_LINE_LENGTH];
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (fgets(input_line, sizeof(input_line), stdin)) {
        if (strlen(input_line) > 0 && input_line[strlen(input_line)-1] == '\n') {
            input_line[strlen(input_line)-1] = '\0';
        }
        
        if (strcmp(input_line, "END") == 0) {
            break;
        }
        
        if (strlen(input_line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(input_line, &entry)) {
            if (!add_log_entry(&logs, &entry)) {
                fprintf(stderr, "Failed to add log entry\n");
                break;
            }
        } else {
            fprintf(stderr, "Failed to parse log line: %s\n", input_line);
        }
        
        if (logs.count >= MAX_ENTRIES) {
            printf("Maximum log entries reached\n");
            break;
        }
    }
    
    print_log_summary(&logs);
    free_log_collection(&logs);
    
    return 0;
}