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
} LogEntry;

typedef struct {
    LogEntry* entries;
    size_t count;
    size_t capacity;
} LogCollection;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
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

void init_log_collection(LogCollection* collection, size_t initial_capacity) {
    if (!collection || initial_capacity == 0) return;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) return;
    
    collection->count = 0;
    collection->capacity = initial_capacity;
}

void add_log_entry(LogCollection* collection, const LogEntry* entry) {
    if (!collection || !entry) return;
    
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity <= collection->capacity) return;
        
        LogEntry* new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    if (collection->count < collection->capacity) {
        collection->entries[collection->count] = *entry;
        collection->count++;
    }
}

void analyze_logs(const LogCollection* collection) {
    if (!collection) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < collection->count; i++) {
        const LogEntry* entry = &collection->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", collection->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (collection->count > 0) {
        printf("First timestamp: %s\n", collection->entries[0].timestamp);
        printf("Last timestamp: %s\n", collection->entries[collection->count - 1].timestamp);
    }
}

void cleanup_log_collection(LogCollection* collection) {
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
    
    char input_buffer[MAX_LINE_LENGTH];
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE). Enter 'END' to finish:\n");
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        if (strncmp(input_buffer, "END", 3) == 0) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(input_buffer, &entry)) {
            add_log_entry(&logs, &entry);
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (logs.count > 0) {
        analyze_logs(&logs);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    cleanup_log_collection(&logs);
    return 0;
}