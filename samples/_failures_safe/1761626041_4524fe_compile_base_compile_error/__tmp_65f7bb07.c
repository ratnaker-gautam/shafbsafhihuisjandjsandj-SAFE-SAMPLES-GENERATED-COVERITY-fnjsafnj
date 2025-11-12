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

int init_log_collection(LogCollection *collection, int capacity) {
    if (!collection || capacity <= 0 || capacity > MAX_ENTRIES) return 0;
    
    collection->entries = malloc(capacity * sizeof(LogEntry));
    if (!collection->entries) return 0;
    
    collection->count = 0;
    collection->capacity = capacity;
    return 1;
}

void free_log_collection(LogCollection *collection) {
    if (collection && collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
        collection->count = 0;
        collection->capacity = 0;
    }
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry || collection->count >= collection->capacity) return 0;
    
    if (collection->count < collection->capacity) {
        collection->entries[collection->count] = *entry;
        collection->count++;
        return 1;
    }
    return 0;
}

void analyze_logs(const LogCollection *collection) {
    if (!collection) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < collection->count; i++) {
        const LogEntry *entry = &collection->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", collection->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (collection->count > 0) {
        printf("First timestamp: %s\n", collection->entries[0].timestamp);
        printf("Last timestamp: %s\n", collection->entries[collection->count - 1].timestamp);
    }
}

int main() {
    LogCollection collection;
    LogEntry stack_entries[10];
    int stack_count = 0;
    
    if (!init_log_collection(&collection, 50)) {
        printf("Failed to initialize log collection\n");
        return 1;
    }
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'quit' to finish input\n");
    
    char input[MAX_LINE_LEN];
    int total_processed = 0;
    
    while (fgets(input, sizeof(input), stdin)) {
        if (strncmp(input, "quit", 4) == 0) break;
        
        input[strcspn(input, "\n")] = 0;
        
        if (strlen(input) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(input, &entry)) {
            if (stack_count < 10) {
                stack_entries[stack_count] = entry;
                stack_count++;
            }
            
            if (!add_log_entry(&collection, &entry)) {
                printf("Warning: Could not add entry to collection\n");
            }
            total_processed++;
        } else {
            printf("Invalid log format: %s\n", input);
        }
        
        if (total_processed >= 100) break;
    }
    
    printf("\nProcessing complete. Analyzed %d entries.\n\n", collection.count);
    analyze_logs(&collection);
    
    printf("\nSample from stack storage (%d entries):\n", stack_count);
    for (int i = 0; i < stack_count && i < 3; i++) {
        printf("%s %s %s\n", stack_entries[i].timestamp, stack_entries[i].level, stack