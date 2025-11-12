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
    size_t count;
    size_t capacity;
} LogCollection;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char *token = strtok(temp, " ");
    if (!token) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (!token) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
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

void count_log_levels(const LogCollection *collection) {
    if (!collection) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < collection->count; i++) {
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

void free_log_collection(LogCollection *collection) {
    if (collection && collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
        collection->count = 0;
        collection->capacity = 0;
    }
}

int main() {
    LogCollection heap_logs;
    init_log_collection(&heap_logs, 10);
    
    LogEntry stack_logs[MAX_ENTRIES];
    size_t stack_count = 0;
    
    char line[MAX_LINE_LEN];
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
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
            if (stack_count < MAX_ENTRIES) {
                stack_logs[stack_count] = entry;
                stack_count++;
            }
            
            add_log_entry(&heap_logs, &entry);
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    printf("\nAnalyzing %zu log entries...\n", heap_logs.count);
    
    if (heap_logs.count > 0) {
        count_log_levels(&heap_logs);
        
        printf("\nFirst 5 log entries:\n");
        size_t display_count = heap_logs.count > 5 ? 5 : heap_logs.count;
        for (size_t i = 0; i < display_count; i++) {
            printf("%s %s %s\n", heap_logs.entries[i].timestamp, 
                   heap_logs.entries[i].level, heap_logs.entries[i].message);
        }
    } else {
        printf("No valid log entries