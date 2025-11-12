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
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int is_valid_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int add_log_entry(LogCollection* collection, const char* line) {
    if (collection->count >= collection->capacity) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (!is_valid_timestamp(timestamp)) return 0;
    
    int severity = parse_severity(level);
    if (severity == -1) return 0;
    
    LogEntry* entry = &collection->entries[collection->count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = severity;
    
    collection->count++;
    return 1;
}

void analyze_logs(LogCollection* collection) {
    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < collection->count; i++) {
        if (collection->entries[i].severity >= 0 && collection->entries[i].severity < 5) {
            severity_count[collection->entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", collection->count);
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", severity_names[i], severity_count[i]);
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
    collection.capacity = MAX_ENTRIES;
    collection.count = 0;
    
    LogEntry stack_entries[MAX_ENTRIES];
    collection.entries = stack_entries;
    
    char input_buffer[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (line_count < MAX_ENTRIES) {
        printf("Log %d: ", line_count + 1);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (strcmp(input_buffer, "END") == 0) {
            break;
        }
        
        if (strlen(input_buffer) == 0)