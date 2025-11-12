//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
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
    size_t count;
    size_t capacity;
} LogCollection;

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_timestamp(const char* timestamp) {
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

int read_log_line(FILE* file, char* buffer, size_t buffer_size) {
    if (!fgets(buffer, buffer_size, file)) return 0;
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
    return 1;
}

int parse_log_entry(const char* line, LogEntry* entry) {
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    if (!timestamp || !validate_timestamp(timestamp)) return 0;
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp)-1);
    entry->timestamp[sizeof(entry->timestamp)-1] = '\0';
    
    char* level = strtok(NULL, " ");
    if (!level) return 0;
    strncpy(entry->level, level, sizeof(entry->level)-1);
    entry->level[sizeof(entry->level)-1] = '\0';
    
    entry->severity = parse_severity(level);
    if (entry->severity == -1) return 0;
    
    char* message = strtok(NULL, "");
    if (!message) return 0;
    strncpy(entry->message, message, sizeof(entry->message)-1);
    entry->message[sizeof(entry->message)-1] = '\0';
    
    return 1;
}

int add_log_entry(LogCollection* collection, const LogEntry* entry) {
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity == 0) new_capacity = 16;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        
        LogEntry* new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
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

void analyze_logs(const LogCollection* collection) {
    int severity_count[5] = {0};
    int total_entries = 0;
    int printed_count = 0;
    
    for (size_t i = 0; i < collection->count; i++) {
        const LogEntry* entry = &collection->entries[i];
        if (entry->severity >= 0 && entry->severity < 5) {
            severity_count[entry->severity]++;
            total_entries++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", total_entries);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("CRITICAL: %d\n", severity_count[4]);
    
    if (total_entries > 0) {
        printf("\nMost severe entries:\n");
        for (int severity = 4; severity >= 0; severity--) {
            for (size_t i = 0; i < collection->count; i++) {
                const LogEntry* entry = &collection->entries[i];
                if (entry->severity == severity) {
                    printf("%s %s %s\n", entry->timestamp, entry->level