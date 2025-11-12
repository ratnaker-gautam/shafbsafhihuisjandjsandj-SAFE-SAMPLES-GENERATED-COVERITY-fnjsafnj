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
    int severity;
} LogEntry;

typedef struct {
    LogEntry *entries;
    int count;
    int capacity;
} LogCollection;

int parse_severity(const char *level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    if (timestamp[4] != '-' || timestamp[7] != '-' || timestamp[10] != ' ' ||
        timestamp[13] != ':' || timestamp[16] != ':') return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (timestamp[i] < '0' || timestamp[i] > '9') return 0;
    }
    return 1;
}

int add_log_entry(LogCollection *collection, const char *timestamp, 
                  const char *level, const char *message) {
    if (collection == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity <= 0) new_capacity = 10;
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (new_entries == NULL) return 0;
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    LogEntry *entry = &collection->entries[collection->count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = parse_severity(level);
    
    if (entry->severity == 0) return 0;
    
    collection->count++;
    return 1;
}

void print_statistics(const LogCollection *collection) {
    if (collection == NULL || collection->count == 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    int severity_count[6] = {0};
    for (int i = 0; i < collection->count; i++) {
        if (collection->entries[i].severity >= 1 && collection->entries[i].severity <= 5) {
            severity_count[collection->entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", collection->count);
    printf("DEBUG: %d\n", severity_count[1]);
    printf("INFO: %d\n", severity_count[2]);
    printf("WARNING: %d\n", severity_count[3]);
    printf("ERROR: %d\n", severity_count[4]);
    printf("CRITICAL: %d\n", severity_count[5]);
}

int main(void) {
    LogCollection collection = {0};
    collection.capacity = 10;
    collection.entries = malloc(collection.capacity * sizeof(LogEntry));
    if (collection.entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Log Analyzer - Enter log entries (timestamp level message)\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL Message text\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'quit' to finish input\n\n");
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_count++;
        if (line_count > 100) {
            printf("Maximum input lines reached\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "quit") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        char timestamp[32] = {0};
        char