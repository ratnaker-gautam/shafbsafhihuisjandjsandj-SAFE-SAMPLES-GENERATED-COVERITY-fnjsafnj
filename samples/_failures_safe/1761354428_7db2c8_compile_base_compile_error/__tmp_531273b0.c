//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

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
    size_t count;
    size_t capacity;
} LogCollection;

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_log_entry(const LogEntry* entry) {
    if (entry == NULL) return 0;
    if (strlen(entry->timestamp) == 0) return 0;
    if (strlen(entry->level) == 0) return 0;
    if (strlen(entry->message) == 0) return 0;
    if (entry->severity < 0 || entry->severity > 4) return 0;
    return 1;
}

int add_log_entry(LogCollection* collection, const LogEntry* entry) {
    if (collection == NULL || entry == NULL) return 0;
    if (!validate_log_entry(entry)) return 0;
    
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity == 0) new_capacity = 16;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        
        LogEntry* new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (new_entries == NULL) return 0;
        
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

void analyze_logs(const LogCollection* collection) {
    if (collection == NULL || collection->count == 0) {
        printf("No logs to analyze.\n");
        return;
    }
    
    int severity_count[5] = {0};
    int total_severity = 0;
    
    for (size_t i = 0; i < collection->count; i++) {
        const LogEntry* entry = &collection->entries[i];
        if (entry->severity >= 0 && entry->severity <= 4) {
            severity_count[entry->severity]++;
            total_severity += entry->severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", collection->count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("CRITICAL: %d\n", severity_count[4]);
    
    if (collection->count > 0) {
        double avg_severity = (double)total_severity / collection->count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

void cleanup_collection(LogCollection* collection) {
    if (collection != NULL) {
        free(collection->entries);
        collection->entries = NULL;
        collection->count = 0;
        collection->capacity = 0;
    }
}

int main(void) {
    LogCollection collection = {0};
    char input_buffer[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log entries (format: timestamp level message)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'analyze' to show results or 'quit' to exit.\n");
    
    while (1) {
        printf("> ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = 0;
        
        if (strcmp(input_buffer, "quit") == 0) {
            break;
        }
        
        if (strcmp(input_buffer, "analyze") == 0) {
            analyze_logs(&collection);
            continue;
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        LogEntry entry;
        char level_buffer[16];
        char message_buffer[256];
        
        int parsed = sscanf(input_buffer, "%31s %15s %255[^\n]", 
                           entry.timestamp, level_buffer, message_buffer);
        
        if (parsed != 3) {
            printf("Invalid format. Use: timestamp level message\n");
            continue;
        }
        
        if (strlen(entry.timestamp) >= sizeof(entry.timestamp) ||
            strlen(level_buffer) >= sizeof(entry.level) ||
            strlen(message_buffer