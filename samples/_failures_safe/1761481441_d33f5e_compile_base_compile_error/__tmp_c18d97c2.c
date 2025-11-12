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
    int severity;
} LogEntry;

typedef struct {
    LogEntry* entries;
    int count;
    int capacity;
} LogCollection;

int parse_severity(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int is_valid_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int add_log_entry(LogCollection* collection, const char* timestamp, const char* level, const char* message) {
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        LogEntry* new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    LogEntry* entry = &collection->entries[collection->count];
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = parse_severity(level);
    
    if (entry->severity == -1) return 0;
    
    collection->count++;
    return 1;
}

void analyze_logs(LogCollection* collection) {
    int severity_count[4] = {0};
    char* levels[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    
    for (int i = 0; i < collection->count; i++) {
        if (collection->entries[i].severity >= 0 && collection->entries[i].severity <= 3) {
            severity_count[collection->entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", collection->count);
    for (int i = 3; i >= 0; i--) {
        printf("%s: %d\n", levels[i], severity_count[i]);
    }
    
    if (collection->count > 0) {
        printf("\nRecent entries:\n");
        int start = collection->count > 5 ? collection->count - 5 : 0;
        for (int i = start; i < collection->count; i++) {
            printf("%s [%s] %s\n", collection->entries[i].timestamp, 
                   collection->entries[i].level, collection->entries[i].message);
        }
    }
}

int main(void) {
    LogCollection collection;
    collection.count = 0;
    collection.capacity = 10;
    collection.entries = malloc(collection.capacity * sizeof(LogEntry));
    if (!collection.entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    char timestamp[32];
    char level[16];
    char message[256];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'quit' to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "quit", 4) == 0) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) == 3) {
            if (!is_valid_timestamp(timestamp)) {
                printf("Invalid timestamp format. Use: YYYY-MM-DD HH:MM:SS\n");
                continue;
            }
            
            if (parse_severity(level) == -1) {
                printf("Invalid log level. Use: DEBUG, INFO, WARN, or ERROR\n");
                continue;
            }
            
            if (!add_log_entry(&collection, timestamp, level, message)) {
                printf("Failed to add log entry\n");
                continue;
            }
            
            printf("Added: %s [%s] %s\n