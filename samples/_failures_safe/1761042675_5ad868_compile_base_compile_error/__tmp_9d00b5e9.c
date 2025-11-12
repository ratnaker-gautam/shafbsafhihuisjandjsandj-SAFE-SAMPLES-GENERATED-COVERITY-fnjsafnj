//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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

int add_log_entry(LogCollection *collection, const char *timestamp, const char *level, const char *message) {
    if (collection == NULL || timestamp == NULL || level == NULL || message == NULL) {
        return 0;
    }
    
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity <= 0) new_capacity = 10;
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (new_entries == NULL) {
            return 0;
        }
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
    
    collection->count++;
    return 1;
}

void analyze_logs(LogCollection *collection) {
    if (collection == NULL || collection->count == 0) {
        printf("No logs to analyze.\n");
        return;
    }
    
    int severity_count[6] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < collection->count; i++) {
        LogEntry *entry = &collection->entries[i];
        if (entry->severity >= 0 && entry->severity <= 5) {
            severity_count[entry->severity]++;
            total_severity += entry->severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", collection->count);
    printf("DEBUG: %d\n", severity_count[1]);
    printf("INFO: %d\n", severity_count[2]);
    printf("WARNING: %d\n", severity_count[3]);
    printf("ERROR: %d\n", severity_count[4]);
    printf("CRITICAL: %d\n", severity_count[5]);
    printf("Unknown: %d\n", severity_count[0]);
    
    if (collection->count > 0) {
        double avg_severity = (double)total_severity / collection->count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

void free_log_collection(LogCollection *collection) {
    if (collection != NULL) {
        free(collection->entries);
        collection->entries = NULL;
        collection->count = 0;
        collection->capacity = 0;
    }
}

int main() {
    LogCollection collection = {0};
    collection.capacity = 10;
    collection.entries = malloc(collection.capacity * sizeof(LogEntry));
    
    if (collection.entries == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    char timestamp[32];
    char level[16];
    char message[256];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'quit' to finish input.\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "quit", 4) == 0) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (result >= 2) {
            if (result == 2) {
                message[0] = '\0';
            }
            
            if (!add_log_entry(&collection, timestamp, level, message)) {
                printf("Failed to add log entry.\n");
            }
        } else {
            printf("Invalid format. Use: