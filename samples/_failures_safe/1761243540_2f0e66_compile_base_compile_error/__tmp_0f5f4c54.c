//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
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
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

LogCollection *create_log_collection(int initial_capacity) {
    if (initial_capacity <= 0) return NULL;
    
    LogCollection *collection = malloc(sizeof(LogCollection));
    if (!collection) return NULL;
    
    collection->entries = malloc(sizeof(LogEntry) * initial_capacity);
    if (!collection->entries) {
        free(collection);
        return NULL;
    }
    
    collection->count = 0;
    collection->capacity = initial_capacity;
    return collection;
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry) return 0;
    
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity <= collection->capacity) return 0;
        
        LogEntry *new_entries = realloc(collection->entries, sizeof(LogEntry) * new_capacity);
        if (!new_entries) return 0;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    collection->entries[collection->count] = *entry;
    collection->count++;
    return 1;
}

void free_log_collection(LogCollection *collection) {
    if (collection) {
        free(collection->entries);
        free(collection);
    }
}

void analyze_log_levels(LogCollection *collection) {
    if (!collection) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < collection->count; i++) {
        if (strcmp(collection->entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(collection->entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(collection->entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Level Analysis:\n");
    printf("  INFO: %d entries\n", info_count);
    printf("  WARNING: %d entries\n", warning_count);
    printf("  ERROR: %d entries\n", error_count);
}

int main(void) {
    LogCollection *collection = create_log_collection(10);
    if (!collection) {
        fprintf(stderr, "Failed to create log collection\n");
        return 1;
    }
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
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
            if (!add_log_entry(collection, &entry)) {
                fprintf(stderr, "Failed to add log entry\n");
                break;
            }
        } else {
            fprintf(stderr, "Failed to parse line: %s\n", line);
        }
    }
    
    if (collection->count > 0) {
        printf("\nAnalysis Results:\n");
        printf("Total entries processed: %d\n", collection->count);
        analyze_log_levels(collection);
        
        printf("\nSample entries:\n");
        int sample_count = collection->count < 3 ? collection->count : 3;
        for (int i = 0; i < sample_count; i++) {
            printf("  %s %s %s\n", 
                   collection->entries[i].timestamp,
                   collection->entries